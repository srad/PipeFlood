#pragma once
#include "GameTypes.h"
#include  "Pipes.h"
#include "Math.h"
#include "Tile.h"
#include "TilePack.h"
#include <queue>
#include <unordered_map>

namespace PipeFlood {
  struct PathInfo {
    std::vector<v2> path;
    std::unordered_map<std::string, bool>  visited;
    std::unordered_map<std::string, v2> parent;
  };

  struct Map {
    v2 size, tileResized;
    v2 resolution;
    v2 target;
    uint16_t margin;
    
    const float tileScale = 1.0f;
    const uint16_t tileSize = 64;
    std::unordered_map<std::string, v2> water;

    uint16_t** field;
    uint16_t** rotation;
    bool** joined;

    JoinsPack joinsPack{ 2 };

    Map(v2 size) : size{ size }, target{ (uint16_t)(size.x - 1), (uint16_t)(size.y - 1) } {
      field = new uint16_t * [size.x];
      rotation = new uint16_t * [size.x];
      joined = new bool* [size.x];

      for (uint16_t x = 0; x < size.x; x++) {
        field[x] = new uint16_t[size.y];
        rotation[x] = new uint16_t[size.y];
        joined[x] = new bool[size.y];
      }

      this->tileResized = v2{ static_cast<uint16_t>(tileSize * tileScale), static_cast<uint16_t>(tileSize * tileScale) };
      this->resolution = v2{ static_cast<uint16_t>(tileResized.x* size.x), static_cast<uint16_t>(tileResized.y* size.y) };

      // Random map
      for (uint16_t x = 0; x < size.x; x++) {
        for (uint16_t y = 0; y < size.y; y++) {
          // Input
          if (x == 0 && y == 0) { field[x][y] = 0; }
          // Output
          else if ((x == (size.x - 1)) && (y == (size.y - 1))) { field[x][y] = 1; }
          else { field[x][y] = Math::rnd(2, vPipeFiles.size() - 1); }
          rotation[x][y] = Math::rnd(0, 3);
#ifdef DEBUG
          std::cout << field[x][y] << "(" << rotation[x][y] << ") ";
#endif
        }
#ifdef DEBUG
        std::cout << std::endl;
#endif
      }

      checkJoins();
    }

    ~Map() {
      delete[] field;
      delete[] rotation;
      delete[] joined;
    }

    void checkJoins() {
      for (uint16_t x = 0; x < size.x; x++) {
        for (uint16_t y = 0; y < size.y; y++) {
          joined[x][y] = doesJoin(v2{ x,y });
        }
      }
    }

    /*
     * Find out if this pipe connects to any adjacent pipe opening.
     */
    bool doesJoin(const v2 pos) {
      const auto& sides = Pipes::vPipeMask[field[pos.x][pos.y]][rotation[pos.x][pos.y]];
      return rightJoin(sides, pos) || leftJoin(sides, pos) || topJoin(sides, pos) || bottomJoin(sides, pos);
    }

    bool leftJoin(const std::vector<bool>& sides, v2 pos) {
      const auto& left = sides[Side::left];

      return (pos.x > 0)
        && left
        && Pipes::vPipeMask[field[pos.x - 1][pos.y]][rotation[pos.x - 1][pos.y]][Side::right];
    }
    
    bool rightJoin(const std::vector<bool>& sides, v2 pos) {
      const auto& right = sides[Side::right];

      return (pos.x < (size.x - 1))
        && right
        && Pipes::vPipeMask[field[pos.x + 1][pos.y]][rotation[pos.x + 1][pos.y]][Side::left];
    }

    bool topJoin(const std::vector<bool>& sides, v2 pos) {
      const auto& top = sides[Side::top];

      return (pos.y > 0)
        && top
        && Pipes::vPipeMask[field[pos.x][pos.y - 1]][rotation[pos.x][pos.y - 1]][Side::bottom];
    }

    bool bottomJoin(const std::vector<bool>& sides, v2 pos) {
      const auto& bottom = sides[Side::bottom];

      return (pos.y < size.y - 1)
        && bottom
        && Pipes::vPipeMask[field[pos.x][pos.y + 1]][rotation[pos.x][pos.y + 1]][Side::top];
    }

    void printJoins() {
#ifdef DEBUG
      for (uint16_t y = 0; y < size.y; y++) {
        for (uint16_t x = 0; x < size.x; x++) {
          std::cout << joined[x][y] << " ";
        }
        std::cout << std::endl;
      }
#endif
    }

    float darkenColor = 0.7f;

    Sprite getSprite(v2 pos, bool active = false) {
      Sprite sprite = joinsPack.toSprite(pos, field[pos.x][pos.y], active, rotation[pos.x][pos.y]);
      if (!joined[pos.x][pos.y] && !active) {
        auto col = sprite.getColor();
        sprite.setColor(sf::Color{ (sf::Uint8)(col.r * darkenColor), (sf::Uint8)(col.g * darkenColor), (sf::Uint8)(col.b * darkenColor) });
      }
      return sprite;
    }

    std::vector<v2> backTraceFrom(PathInfo& pathInfo, v2 from, v2 to) {
      std::vector<v2> path;
      path.push_back(from);

      auto it = pathInfo.parent.find(from.str());
      path.push_back(it->second);
      while (it->second.x != to.x && it->second.y != to.y) {
        it = pathInfo.parent.find(it->second.str());
        path.push_back(it->second);
      }
      path.push_back(to);

      return path;
    }

    PathInfo BFS(v2 start, v2 goal) {
      auto pathInfo = PathInfo{};
      std::queue<v2> queue{};
      auto path = BFS(start, goal, queue, pathInfo);

      return pathInfo;
    }

    PathInfo BFS(v2 start, v2 goal, std::queue<v2> queue, PathInfo& pathInfo) {
      queue.push(start);

      while (!queue.empty()) {
        auto v = queue.front();
        queue.pop();

        if (v.str() == goal.str()) {
          return pathInfo;
        }

        // Does this pipe connect to any other?
        bool joins = joined[v.x][v.y];

        // insert all adjacent points
        if (joins) {
          const auto& sides = Pipes::vPipeMask[field[v.x][v.y]][rotation[v.x][v.y]];
          auto left = v2{ (uint16_t)(v.x - 1), v.y };
          auto right = v2{ (uint16_t)(v.x + 1), v.y };
          auto top = v2{ v.x, (uint16_t)(v.y - 1) };
          auto bottom = v2{ v.x, (uint16_t)(v.y + 1) };

          if (!pathInfo.visited[left.str()] && leftJoin(sides, v)) {
            pathInfo.visited[left.str()] = true;
            queue.push(left);
            pathInfo.parent[left.str()] = v;
          }

          if (!pathInfo.visited[right.str()] && rightJoin(sides, v)) {
            pathInfo.visited[right.str()] = true;
            queue.push(right);
            pathInfo.parent[right.str()] = v;
          }

          if (!pathInfo.visited[top.str()] && topJoin(sides, v)) {
            pathInfo.visited[top.str()] = true;
            queue.push(top);
            pathInfo.parent[top.str()] = v;
          }

          if (!pathInfo.visited[bottom.str()] && bottomJoin(sides, v)) {
            pathInfo.visited[bottom.str()] = true;
            queue.push(bottom);
            pathInfo.parent[bottom.str()] = v;
          }
        }
      }

      return pathInfo;
    }
  };
}