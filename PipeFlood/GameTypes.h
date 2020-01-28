#pragma once
#include <cstdint>
#include <string>
#include <SFML\Graphics\Sprite.hpp>

namespace PipeFlood {
  
  struct v2 {
    uint16_t x, y;
    std::string str() {
      return "v2(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
    v2 operator *(const v2& rhs) {
      return v2{ static_cast<uint16_t>(x * rhs.x), static_cast<uint16_t>(y * rhs.y) };
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