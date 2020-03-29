#pragma once
#include "GameTypes.h"
#include  "resources/Pipes.h"
#include "Math.h"
#include "resources/Tile.h"
#include "resources/TilePack.h"
#include <queue>

namespace StarPipe {
  struct Map {
    Vec2 target;

    // Unjoined pipes are drawn darker
    const float darkenColor = 0.7f;

    Field field;
    Rotations rotation;
    BitField joined;
    
    GameInfo gameInfo;
    JoinsPack joinsPack{ GameInfo::PipePack };

    Map(GameInfo gameInfo) :
      gameInfo{ gameInfo },
      field(gameInfo.mapSize.x, std::vector<field_t>(gameInfo.mapSize.y, 0)),
      rotation(gameInfo.mapSize.x, std::vector<rot_t>(gameInfo.mapSize.y, 0)),
      joined(gameInfo.mapSize.x, std::vector<bool>(gameInfo.mapSize.y, false)),
      target{ (grid_t)(gameInfo.mapSize.x - 1), (grid_t)(gameInfo.mapSize.y - 1) } {

      // Random map
      for (grid_t x = 0; x < gameInfo.mapSize.x; x++) {
        for (grid_t y = 0; y < gameInfo.mapSize.y; y++) {
          rotation[x][y] = 0;
          // Start
          if (x == 0 && y == 0) { field[x][y] = 0; }
          // End
          else if ((x == (gameInfo.mapSize.x - 1)) && (y == (gameInfo.mapSize.y - 1))) {
            field[x][y] = 1;
          }
          // Random tiles
          else {
            auto r = Math::rnd(0, 100);
            field_t type = TileType::None;
            if (r <= 5) { type = TileType::None; }
            else if (r <= 10) { type = TileType::Start; }
            else if (r <= 15) { type = TileType::End; }
            else if (r <= 40) { type = TileType::Edge; }
            else if (r <= 60) { type = TileType::I; }
            else if (r <= 90) { type = TileType::T; }
            else if (r <= 100) { type = TileType::None2; }
            //else if (r <= 99) { type = TileType::Void; }
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
      field.clear();
      rotation.clear();
      joined.clear();
    }

    void checkJoins() {
      for (grid_t x = 0; x < gameInfo.mapSize.x; x++) {
        for (grid_t y = 0; y < gameInfo.mapSize.y; y++) {
          joined[x][y] = doesJoin(Vec2{ x,y });
        }
      }
    }

    /*
     * Find out if this pipe connects to any adjacent pipe opening.
     */
    bool doesJoin(const Vec2 pos) {
      const auto& sides = Pipes::vPipeMask[field[pos.x][pos.y]][rotation[pos.x][pos.y]];
      return rightJoin(sides, pos) || leftJoin(sides, pos) || topJoin(sides, pos) || bottomJoin(sides, pos);
    }

    bool leftJoin(const std::vector<bool>& sides, Vec2 pos) {
      const auto& left = sides[Side::left];

      return (pos.x > 0)
        && left
        && Pipes::vPipeMask[field[pos.x - 1][pos.y]][rotation[pos.x - 1][pos.y]][Side::right];
    }

    bool rightJoin(const std::vector<bool>& sides, Vec2 pos) {
      const auto& right = sides[Side::right];

      return (pos.x < (gameInfo.mapSize.x - 1))
        && right
        && Pipes::vPipeMask[field[pos.x + 1][pos.y]][rotation[pos.x + 1][pos.y]][Side::left];
    }

    bool topJoin(const std::vector<bool>& sides, Vec2 pos) {
      const auto& top = sides[Side::top];

      return (pos.y > 0)
        && top
        && Pipes::vPipeMask[field[pos.x][pos.y - 1]][rotation[pos.x][pos.y - 1]][Side::bottom];
    }

    bool bottomJoin(const std::vector<bool>& sides, Vec2 pos) {
      const auto& bottom = sides[Side::bottom];

      return (pos.y < gameInfo.mapSize.y - 1)
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

    Sprite getSprite(Vec2 pixelPos, Vec2 pos, bool active = false) {
      Sprite sprite = joinsPack.toSprite(pixelPos, field[pos.x][pos.y], active, rotation[pos.x][pos.y]);
      if (!joined[pos.x][pos.y] && !active) {
        auto col = sprite.getColor();
        sprite.setColor(sf::Color{ (sf::Uint8)(col.r * darkenColor), (sf::Uint8)(col.g * darkenColor), (sf::Uint8)(col.b * darkenColor) });
      }
      return sprite;
    }
    
    TraceInfo backTraceFrom(PathInfo& pathInfo, Vec2 from, Vec2 to) {
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

    PathInfo BFS(Vec2 start, Vec2 goal, std::queue<Vec2> queue = std::queue<Vec2>{}, PathInfo pathInfo = PathInfo{}) {
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
          auto left = Vec2{ (grid_t)(v.x - 1), v.y };
          auto right = Vec2{ (grid_t)(v.x + 1), v.y };
          auto top = Vec2{ v.x, (grid_t)(v.y - 1) };
          auto bottom = Vec2{ v.x, (grid_t)(v.y + 1) };

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