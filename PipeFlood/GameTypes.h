#pragma once
#include <cstdint>
#include <string>
#include <SFML\Graphics\Sprite.hpp>
#include <unordered_map>
#include <string>

namespace PipeFlood {
  typedef uint16_t grid_t;
  typedef uint8_t field_t;
  typedef uint8_t rot_t;
  typedef std::vector<std::vector<field_t>> Field;  
  typedef std::vector<std::vector<bool>> BitField;  
  typedef std::vector<std::vector<rot_t>> Rotations;
  
  struct Vec2 {
    grid_t x, y;

    Vec2() : x{ 0 }, y{ 0 } {}
    Vec2(float x, float y) : x{ (grid_t)x }, y{ (grid_t)y } {}
    Vec2(int x, int y) : x{ (grid_t)x }, y{ (grid_t)y } {}

    std::string str() {
      return "Vec2(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
    Vec2 operator *(const Vec2& rhs) {
      return Vec2{ static_cast<uint16_t>(x * rhs.x), static_cast<uint16_t>(y * rhs.y) };
    }
    bool operator==(const Vec2& other) const {
      return x == other.x && y == other.y;
    }
  };

  struct GameInfo {
    static const uint16_t TilePack = 2;
    static const uint16_t PipePack = 1;

    Vec2 tileSize;
    Vec2 mapSize;
    Vec2 screenSize;
    uint16_t margin;
    Vec2 resolution;
    GameInfo(Vec2 tileSize, Vec2 mapSize, uint16_t margin) :
      tileSize{ tileSize },
      mapSize{ mapSize },
      margin{ margin },
      screenSize{ Vec2{(grid_t)(mapSize.x + (margin * 2 + 2)),(grid_t)(mapSize.y + (margin * 2 + 2))} },
      resolution{ Vec2{(grid_t)(tileSize.x * (mapSize.x + 2 * margin + 2)), (grid_t)(tileSize.y * (mapSize.y + 2 * margin + 2))} } {};
  };

  typedef std::vector<Vec2> Path;

  struct PathInfo {
    Path path;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, Vec2> parent;
  };

  struct TraceInfo {
    Path path;
    std::unordered_map<std::string, bool> visited;
    void add(Vec2 v) {
      path.push_back(v);
      visited[v.str()] = true;
    }
    void add(std::string v) {
      visited[v] = true;
    }
  };

  typedef sf::Sprite Sprite;
  typedef sf::Texture Tex;
  typedef std::vector<Sprite> Sprites;

  struct InputInfo {
    mutable bool mouseDown = false;
    mutable bool keyDown = false;
  };

  struct Side { static const uint16_t left = 0, right = 1, top = 2, bottom = 3; };
}