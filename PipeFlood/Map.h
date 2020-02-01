#pragma once
#include "GameTypes.h"
#include  "Pipes.h"
#include "Math.h"
#include "Tile.h"
#include "TilePack.h"
#include <queue>

namespace PipeFlood {
  struct Map {
    v2 target;

    // Unjoined pipes are drawn darker
    const float darkenColor = 0.6f;

    uint16_t** field;
    uint16_t** rotation;
    bool** joined;

    GameInfo gameInfo;
    JoinsPack joinsPack{ 2 };

    Map(GameInfo gameInfo) : gameInfo{ gameInfo }, target{ (uint16_t)(gameInfo.boardSize.x - 1), (uint16_t)(gameInfo.boardSize.y - 1) } {
      field = new uint16_t * [gameInfo.boardSize.x];
      rotation = new uint16_t * [gameInfo.boardSize.x];
      joined = new bool* [gameInfo.boardSize.x];

      for (uint16_t x = 0; x < gameInfo.boardSize.x; x++) {
        field[x] = new uint16_t[gameInfo.boardSize.y];
        rotation[x] = new uint16_t[gameInfo.boardSize.y];
        joined[x] = new bool[gameInfo.boardSize.y];
      }

      // Random map
      for (uint16_t x = 0; x < gameInfo.boardSize.x; x++) {
        for (uint16_t y = 0; y < gameInfo.boardSize.y; y++) {
          rotation[x][y] = 0;
          // Input
          if (x == 0 && y == 0) { field[x][y] = 0; }
          // Output
          else if ((x == (gameInfo.boardSize.x - 1)) && (y == (gameInfo.boardSize.y - 1))) {
            field[x][y] = 1;
          }
          // Pipes
          else {
            auto r = Math::rnd(0, 100);
            uint16_t type = TileType::None;
            if (r <= 3) { type = TileType::Start; }
            else if (r <= 6) { type = TileType::End; }
            else if (r <= 35) { type = TileType::Edge; }
            else if (r <= 60) { type = TileType::I; }
            else if (r <= 90) { type = TileType::T; }
            else if (r <= 97) { type = TileType::None2; }
            field[x][y] = type;

            if (field[x][y] != TileType::None && field[x][y] != TileType::None2) {
              rotation[x][y] = Math::rnd(0, 3);
            }
          }
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
      for (uint16_t x = 0; x < gameInfo.boardSize.x; x++) {
        for (uint16_t y = 0; y < gameInfo.boardSize.y; y++) {
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

      return (pos.x < (gameInfo.boardSize.x - 1))
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

      return (pos.y < gameInfo.boardSize.y - 1)
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

    Sprite getSprite(v2 pos, bool active = false) {
      Sprite sprite = joinsPack.toSprite(pos, field[pos.x][pos.y], active, rotation[pos.x][pos.y]);
      if (!joined[pos.x][pos.y] && !active) {
        auto col = sprite.getColor();
        sprite.setColor(sf::Color{ (sf::Uint8)(col.r * darkenColor), (sf::Uint8)(col.g * darkenColor), (sf::Uint8)(col.b * darkenColor) });
      }
      return sprite;
    }
    
    TraceInfo backTraceFrom(PathInfo& pathInfo, v2 from, v2 to) {
      TraceInfo trace;
      trace.add(from);

      auto it = pathInfo.parent.find(from.str());
      trace.path.push_back(it->second);
      while (it->second.x != to.x && it->second.y != to.y) {
        it = pathInfo.parent.find(it->second.str());
        trace.add(it->second);
      }
      trace.add(to);

      return trace;
    }

    PathInfo BFS(v2 start, v2 goal, std::queue<v2> queue = std::queue<v2>{}, PathInfo pathInfo = PathInfo{}) {
      queue.push(start);

      while (!queue.empty()) {
        auto v = queue.front();
        queue.pop();

        if (v == goal) {
          return pathInfo;
        }

        // Does this pipe connect to any other?
        // insert all adjacent points
        if (joined[v.x][v.y]) {
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