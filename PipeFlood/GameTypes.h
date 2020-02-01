#pragma once
#include <cstdint>
#include <string>
#include <SFML\Graphics\Sprite.hpp>
#include <unordered_map>
#include <string>

namespace PipeFlood {
  
  struct v2 {
    uint16_t x, y;
    std::string str() {
      return "v2(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
    v2 operator *(const v2& rhs) {
      return v2{ static_cast<uint16_t>(x * rhs.x), static_cast<uint16_t>(y * rhs.y) };
    }
    bool operator==(const v2& other) const {
      return x == other.x && y == other.y;
    }
    v2 swap() {
      return v2{ y, x };
    }
  };

  struct GameInfo {
    v2 tileSize;
    v2 boardSize;
    uint16_t margin;
    v2 resolution;
    GameInfo(v2 tileSize, v2 boardSize, uint16_t margin) :
      tileSize{ tileSize },
      boardSize{ boardSize },
      margin{ margin },
      resolution{ v2{(uint16_t)(tileSize.x * (boardSize.x + 2 * margin)), (uint16_t)(tileSize.y * (boardSize.y + 2 * margin))} } {};
};

  typedef std::vector<v2> Path;

  struct PathInfo {
    Path path;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, v2> parent;
  };

  struct TraceInfo {
    Path path;
    std::unordered_map<std::string, bool> visited;
    void add(v2 v) {
      path.push_back(v);
      visited[v.str()] = true;
    }
    void add(std::string v) {
      visited[v] = true;
    }
  };

  typedef sf::Sprite Sprite;
  typedef sf::Texture Tex;

  struct InputInfo {
    mutable bool mouseDown = false;
    mutable bool keyDown = false;
  };

  struct Side { static const uint16_t left = 0, right = 1, top = 2, bottom = 3; };
}