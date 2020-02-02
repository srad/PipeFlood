#pragma once
#include <vector>
#include <cstdint>
#include "GameTypes.h"
#include <string>
#include <iostream>
#include <SFML\System\Vector2.hpp>

namespace PipeFlood {
  // Must correspond to the Pipefiles vector
  enum TileType { Start = 0, End = 1, I = 2, T = 3, Edge = 4, None = 5, None2 = 6, Void = 7 };
  struct TileInfo { std::string filename; TileType type; };

  const std::vector<TileInfo> vPipeFiles
  {
    //"cross5.png",
    TileInfo{"opening", TileType::Start},
    TileInfo{"opening_out", TileType::End},
    TileInfo{"i",  TileType::I},
    TileInfo{"t",  TileType::T},
    TileInfo{"edge",  TileType::Edge},
    TileInfo{"none",  TileType::None},
    TileInfo{"none2",  TileType::None2},
    TileInfo{"void",  TileType::Void}
  };

  struct Tkile {
    TileType type;
    const uint16_t tileSize = 64;
    std::string filename;
    Sprite sprite;
    Tex tex;
    const float scale = 1.0f;
    const std::string path = "C:/Users/saman/src/PipeFlood/PipeFlood/assets/textures/";

    Tkile(const std::string filename, TileType type) : filename{ filename }, type{ type } {
      const auto file = path + filename;
      const auto loaded = tex.loadFromFile(file);
      if (!loaded) { throw; }
      tex.setSmooth(true);
      sprite.setTexture(tex);
    }

    Sprite createSprite() {
      Sprite sprite;
      sprite.setTexture(tex);
      sprite.setPosition(sf::Vector2f(0, 0));
      return sprite;
    }

    Sprite createSprite(Vec2 pos, Vec2 size, rot_t rotation) {
      Sprite sprite;

      sprite.setTexture(tex);
      float pixelX = pos.x * size.x, pixelY = pos.y * size.y;
      sprite.setPosition(sf::Vector2f(pixelX + size.x / 2, pixelY + size.y / 2));
      sprite.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
      sprite.setRotation(rotation * 90.0f);
      sprite.setScale(sf::Vector2f(scale, scale));

      return sprite;
    }
  };
}
