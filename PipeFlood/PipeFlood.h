#pragma once
#include "GameTypes.h";

namespace PipeFlood {
  const enum side { left = 0, right = 1, top = 2, bottom = 3 };

  // Each vector defines the each side of the square 
  // in the order given by the enum "side". Only 3 bits for each side is needed,
  // so this declaration could be more compact, but meh.

  const bool pipe = true;
  const bool none = false;

  const std::vector<std::vector<std::vector<bool>>> vBoolMask
  {
    // From left to right each element with a rotation clockwise each step by pi/2

    // +---+---+---+
    // |   | X |   |
    // +---+---+---+
    // | X | X | X |
    // +---+---+---+
    // |   | X |   |
    // +---+---+---+
    /*
    std::vector<std::vector<bool>>
    {
      {pipe, pipe, pipe, pipe },
      {pipe, pipe, pipe, pipe },
      {pipe, pipe, pipe, pipe },
      {pipe, pipe, pipe, pipe }
    },
    */
    // +---+---+---+ +---+---+---+
    // |   | X |   | |   |   |   |
    // +---+---+---+ +---+---+---+
    // |   | X |   | | X | X | X |
    // +---+---+---+ +---+---+---+
    // |   | X |   | |   |   |   |
    // +---+---+---+ +---+---+---+
    std::vector<std::vector<bool>>
    {
      {none, none, pipe, pipe },
      {pipe, pipe, none, none },
      {none, none, pipe, pipe },
      {pipe, pipe, none, none }
    },

    // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
    // |   |   |   | |   | X |   | |   | X |   | |   | X |   |
    // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
    // | X | X | X | | X | X |   | | X | X | X | |   | X | X |
    // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
    // |   | X |   | |   | X |   | |   |   |   | |   | X |   |
    // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
    std::vector<std::vector<bool>>
    {
      { pipe, pipe, none, pipe },
      { pipe, none, pipe, pipe },
      { pipe, pipe, pipe, none },
      { none, pipe, pipe, pipe },
    },

        // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
        // |   |   |   | |   |   |   | |   | X |   | |   | X |   |
        // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
        // |   | X | X | | X | X |   | | X | X |   | |   | X | X |
        // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
        // |   | X |   | |   | X |   | |   |   |   | |   |   |   |
        // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
        std::vector<std::vector<bool>>
        {
          { none, pipe, none, pipe },
          { pipe, none, none, pipe },
          { pipe, none, pipe, none },
          { none, pipe, pipe, none },
        }
  };

  inline uint16_t rnd(uint16_t min, uint16_t max) {
    return rand() % (max - min + 1) + min;
  }

  struct Tile {
    const uint16_t tileSize = 64;
    std::string filename;
    sf::Sprite sprite;
    sf::Texture tex;
    const float scale = 1.0f;
    const std::string path = "C:/Users/saman/src/PipeFlood/PipeFlood/assets/textures/";

    Tile(const std::string filename) {
      this->filename = filename;
      if (!tex.loadFromFile(path + filename)) {
        throw;
      }
      tex.setSmooth(true);
      std::cout << tex.getSize().x << ", " << tex.getSize().y << std::endl;
      sprite.setTexture(tex);
      sprite.setScale(sf::Vector2f(scale, scale));
    }
  };

  struct Map {
    v2 size, tileResized;
    v2 resolution;

    const float tileScale = 1.0f;
    const uint16_t tileSize = 64;

    uint16_t** field;
    uint16_t** rotation;

    const uint16_t tileSet = 5;
    const std::vector<std::string> vPipeFiles{/*"cross5.png",*/ "i", "t", "edge" };

    Tile selector;
    std::vector<PipeFlood::Tile> vTiles;
    std::vector<PipeFlood::Tile> vWaterTiles;

    Map(v2 size) :
      // Doesn't initialize
      // field{ std::vector<std::vector<uint16_t>>{ sizeX } },
      // rotation{ std::vector<std::vector<uint16_t>>{ sizeX } },
      size{ size },
      selector{ PipeFlood::Tile("selector.png") } {
      field = new uint16_t*[size.x];
      rotation = new uint16_t*[size.x];
      for (uint16_t x = 0; x < size.x; x++) {
        field[x] = new uint16_t[size.y];
        rotation[x] = new uint16_t[size.y];
      }
      std::cout << "size: " << size.x << ", " << size.y << std::endl;
      //std::cout << "vector:" << field.size() << ", " << field[0].size() << std::endl;

      this->tileResized = v2{ static_cast<uint16_t>(tileSize * tileScale), static_cast<uint16_t>(tileSize * tileScale) };
      this->resolution = v2{ static_cast<uint16_t>(tileResized.x * size.x), static_cast<uint16_t>(tileResized.y * size.y) };

      for (auto file : vPipeFiles) {
        vTiles.push_back(PipeFlood::Tile(file + std::to_string(tileSet) + ".png"));
        vWaterTiles.push_back(PipeFlood::Tile(file + std::to_string(tileSet) + "_water.png"));
      }

      for (uint16_t x = 0; x < size.x; x++) {
        for (uint16_t y = 0; y < size.y; y++) {
          field[x][y] = PipeFlood::rnd(0, vPipeFiles.size() - 1);
          // {0,1,2,3} * 90°
          rotation[x][y] = PipeFlood::rnd(0, 3);
          std::cout << field[x][y] << "(" << rotation[x][y] << ") ";
        }
        std::cout << std::endl;
      }
    }

    ~Map() {
      delete[] field;
      delete[] rotation;
    }

    /**
     * Find out if this pipe connects to any adjacent pipe opening.
     **/
    bool doesJoin(uint16_t x, uint16_t y) {
      const auto& sides = PipeFlood::vBoolMask[field[x][y]][rotation[x][y]];
      const auto& top = sides[PipeFlood::side::top];
      const auto& bottom = sides[PipeFlood::side::bottom];
      const auto& left = sides[PipeFlood::side::left];
      const auto& right = sides[PipeFlood::side::right];

      const bool leftMatch = (x > 0)
        && left
        && PipeFlood::vBoolMask[field[x - 1][y]][rotation[x - 1][y]][PipeFlood::side::right];

      const bool rightMatch = (x < (size.x - 1))
        && right
        && PipeFlood::vBoolMask[field[x + 1][y]][rotation[x + 1][y]][PipeFlood::side::left];

      const bool topMatch = (y > 0)
        && top
        && PipeFlood::vBoolMask[field[x][y - 1]][rotation[x][y - 1]][PipeFlood::side::bottom];

      const bool bottomMatch = (y < size.y - 1)
        && bottom
        && PipeFlood::vBoolMask[field[x][y + 1]][rotation[x][y + 1]][PipeFlood::side::top];

      return rightMatch || leftMatch || topMatch || bottomMatch;
    }

    Sprite tile(v2 pos) {
      Sprite sprite = doesJoin(pos.x, pos.y) ? vWaterTiles[field[pos.x][pos.y]].sprite : vTiles[field[pos.x][pos.y]].sprite;
      
      float pixelX = pos.x * tileResized.x, pixelY = pos.y * tileResized.y;
      sprite.setPosition(sf::Vector2f(pixelX + tileResized.x / 2, pixelY + tileResized.y / 2));
      sprite.setOrigin(sf::Vector2f(tileResized.x / 2, tileResized.y / 2));
      sprite.setRotation(rotation[pos.x][pos.y] * 90.0f);
      
      return sprite;
    }
  };
}