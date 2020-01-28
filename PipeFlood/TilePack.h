#pragma once
#include "Tile.h"
#include "GameTypes.h"
#include <SFML\System\Vector2.hpp>

namespace PipeFlood {

  struct TileMap {
  private:
    const std::string path = "../PipeFlood/assets/textures/";
    Tex tex;

  public:
    Sprite sprite;

    TileMap(std::string filename) {
      if (!tex.loadFromFile(path + filename)) {
        std::cerr << "could not load: " << path + filename << std::endl;
      }
      sprite.setTexture(tex);
    }

    Sprite toSprite() {
      Sprite sprite;
      sprite.setTexture(tex);
      return sprite;
    }
  };

  struct MapTile : public TileMap {
    MapTile(std::string filename, uint16_t pack) : TileMap{ "maps/" + std::to_string(pack) + "/" + filename } {}
  };

  struct JoinTile {
    TileMap normal;
    TileMap active;

    JoinTile(std::string filename, uint16_t pack) :
      normal{ "pipes/" + std::to_string(pack) + "/" + filename },
      active{ "pipes/" + std::to_string(pack) + "/water/" + filename } {}
  };

  struct Pack {
    uint16_t pack;
    Pack(uint16_t pack) : pack{ pack } {}
  };

  struct JoinsPack : public Pack {
    JoinsPack(uint16_t pack) : Pack{ pack } {}

    Sprite toSprite(v2 pos, uint16_t index, bool active, uint16_t rotation, v2 size = v2{ 64, 64 }) {
      Sprite sprite = active
        ? connections[index].active.toSprite()
        : connections[index].normal.toSprite();

      float pixelX = pos.x * size.x, pixelY = pos.y * size.y;
      sprite.setPosition(sf::Vector2f(pixelX + size.x / 2, pixelY + size.y / 2));
      sprite.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
      sprite.setRotation(rotation * 90.0f);
      //sprite.setScale(sf::Vector2f(scale, scale));

      return sprite;
    }

  private:
    JoinTile connections[5] =
    {
      JoinTile{ "opening.png", pack },
      JoinTile{ "opening_out.png", pack },
      JoinTile{ "i.png",  pack },
      JoinTile{ "t.png",  pack },
      JoinTile{ "edge.png",  pack }
    };
  };

  struct TilePack : public Pack {
    TilePack(uint16_t pack) : Pack{ pack } {}
    
    MapTile bg{ "bg.png", pack };
    MapTile bottom{ "bottom.png", pack };
    MapTile bottom_left{ "bottom_left.png", pack };
    MapTile bottom_right{ "bottom_right.png", pack };
    MapTile center{ "center.png", pack };
    MapTile left{ "left.png", pack };
    MapTile left_top{ "left_top.png", pack };
    MapTile right{ "right.png", pack };
    MapTile top{ "top.png", pack };
    MapTile top_right{ "top_right.png", pack };

    const Sprite* spriteGrid(uint16_t x, uint16_t y, uint16_t lastX, uint16_t lastY, sf::Vector2f pos) {
      Sprite* s = &center.sprite;
      if (x == 0 && y == 0) { s = &left_top.sprite; }
      else if (y == 0 && x < lastX && x > 0) { s = &top.sprite; }
      else if (y == 0 && x == lastX) { s = &top_right.sprite; }
      else if (x == lastX && y == lastY) { s = &bottom_right.sprite; }
      else if (x == 0 && y > 0 && y < lastY) { s = &left.sprite; }
      else if (x > 0 && y > 0 && y < lastY && x < lastX) { s = &center.sprite; }
      else if (x == lastX && y > 0 && y < lastY) { s = &right.sprite; }
      else if (x > 0 && x < lastX && y == lastY) { s = &bottom.sprite; }
      else if (x == 0 && y == lastY) { s = &bottom_left.sprite; }
      s->setPosition(pos);
      return s;
    }
  };

}