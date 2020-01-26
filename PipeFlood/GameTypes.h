#pragma once

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

  struct InputInfo {
    mutable bool mouseDown = false;
    mutable bool keyDown = false;
  };

}