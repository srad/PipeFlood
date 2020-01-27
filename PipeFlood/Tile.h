#pragma once
#include <vector>
#include <cstdint>
#include "GameTypes.h";

namespace PipeFlood {
  namespace Tiles {
    enum TileType { Input, Output, Default };
    struct TileInfo { std::string filename; TileType type; };

    const std::vector<TileInfo> vPipeFiles
    {
      //"cross5.png",
      TileInfo{"opening", TileType::Input},
      TileInfo{"opening_out", TileType::Output},
      TileInfo{"i",  TileType::Default},
      TileInfo{"t",  TileType::Default},
      TileInfo{"edge",  TileType::Default}
    };

    const uint16_t tilePack = 5;

    
    struct Tile {
      TileType type;
      const uint16_t tileSize = 64;
      std::string filename;
      sf::Sprite sprite;
      sf::Texture tex;
      const float scale = 1.0f;
      const std::string path = "C:/Users/saman/src/PipeFlood/PipeFlood/assets/textures/";

      Tile(const std::string filename) {
        Tile(filename, TileType::Default);
      }

      Tile(const std::string filename, TileType type) : filename{ filename }, type{ type } {
        if (!tex.loadFromFile(path + filename)) { throw; }
        tex.setSmooth(true);
        sprite.setTexture(tex);
      }

      Sprite createSprite() {
        Sprite sprite;
        sprite.setTexture(tex);
        sprite.setPosition(sf::Vector2f(0, 0));
        return sprite;
      }

      Sprite createSprite(v2 pos, v2 size, uint16_t rotation) {
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
}
