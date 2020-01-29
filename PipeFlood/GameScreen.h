#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.h"
#include "Tile.h"
#include "Screen.h"
#include "TilePack.h"

namespace PipeFlood {
  typedef std::vector<Sprite> Sprites;

  class GameScreen : public Screen {
  public:
    v2 resolution;

    TileMap selector{ "selector.png" };
    TilePack tilePack{ 1 };
    Sprites mapSprites;
    Sprites connectors;

    PipeFlood::PathInfo pathInfo;
    bool won = false;

    GameScreen(v2 size, std::function<void()> keypressed) :
      Screen(size, keypressed),
      map{ size },
      resolution{ static_cast<uint16_t>(size.x * 64), static_cast<uint16_t>(size.y * 64) } {
    };

    void key(sf::RenderWindow* window, sf::Event* event, PipeFlood::InputInfo* input) {
      switch (event->key.code) {
      case sf::Keyboard::Enter: rotate(cursor); break;
      case sf::Keyboard::Right: right(); break;
      case sf::Keyboard::Up: up(); break;
      case sf::Keyboard::Down: down(); break;
      case sf::Keyboard::Left: left(); break;
      }
      selector.sprite.setPosition(sf::Vector2f(cursor.x * map.tileResized.x, cursor.y * map.tileResized.y));
    }

    void mouse(sf::RenderWindow* window, sf::Event* event, PipeFlood::InputInfo* input) {
      auto pos = sf::Mouse::getPosition(*window);
      cursor.x = pos.x / map.tileResized.x;
      cursor.y = pos.y / map.tileResized.y;
      rotate(cursor);
      selector.sprite.setPosition(sf::Vector2f( (float)(cursor.x * map.tileResized.x), (float)(cursor.y * map.tileResized.y)));
    }

    void update(float delta) {};

    void create(sf::RenderWindow* window) {
      traverse([this, window](uint16_t x, uint16_t y, uint16_t lastX, uint16_t lastY, float pixelX, float pixelY) {
        const auto v = v2{ x, y };
        mapSprites.push_back(*tilePack.spriteGrid(x, y, lastX, lastY, sf::Vector2f{ pixelX, pixelY }));
        auto sprite = map.getSprite(v, false);
        connectors.push_back(sprite);
      });
    };

    void resize(sf::RenderWindow* window) {};

    inline void traverse(std::function<void(uint16_t, uint16_t, uint16_t, uint16_t, float, float)> callback) {
      for (uint16_t y = 0; y < map.size.y; y++) {
        for (uint16_t x = 0; x < map.size.x; x++) {
          float pixelX = x * map.tileSize;
          float pixelY = y * map.tileSize;
          callback(x, y, map.size.x - 1, map.size.y - 1, pixelX, pixelY);
        }
      }
    }

    void draw(sf::RenderWindow* window, float delta) {
      window->clear(sf::Color::Black);

      traverse([this, window](uint16_t x, uint16_t y, uint16_t lastX, uint16_t lastY, float pixelX, float pixelY) {
        tilePack.bg.sprite.setPosition(sf::Vector2f(pixelX, pixelY));
        window->draw(tilePack.bg.sprite);
        auto index = (size.x * y) + x;
        window->draw(mapSprites[index]);
        window->draw(connectors[index]);
      });

      window->draw(selector.sprite);

      // Back traverse father array to start
      if (won) {
        for (auto& field : backtrace) {
          auto sprite = map.getSprite(field);
          sprite.setColor(sf::Color{ 200, 100, 100 });
          window->draw(sprite);
        }
      }
    }

    void close(sf::RenderWindow* window) {}

  private:
    Map map;
    v2 cursor = v2{ 0, 0 };
    std::vector<v2> backtrace;

    void rotate(v2 pos) {
      won = false;
      map.rotation[cursor.x][cursor.y] = (map.rotation[cursor.x][cursor.y] + 1) % 4;

#ifdef DEBUG
      std::cout << cursor.str() << " " << map.rotation[cursor.x][cursor.y] << std::endl;
#endif

      map.checkJoins();
      map.printJoins();

      // Found connection
      pathInfo = map.BFS(v2{ 0,0 }, map.target);
      if (!won && pathInfo.visited[map.target.str()]) {
        backtrace = map.backTraceFrom(pathInfo, map.target, v2{ 0, 0 });
        won = true;
      }

      for (uint16_t y = 0; y < size.y; y++) {
        for (uint16_t x = 0; x < size.x; x++) {
          auto index = (size.x * y) + x;
          connectors[index] = map.getSprite(v2{ x, y }, pathInfo.visited[v2{ x,y }.str()]);
        }
      }

#ifdef DEBUG
      for (auto& p : pathInfo.path) {
        std::cout << p.str() << " ";
      }
      std::cout << std::endl;

      for (auto& p : pathInfo.parent) {
        std::cout << "(" << p.second.str() << ", " << p.first << ") ";
      }
      std::cout << std::endl;

      if (won) {
        for (auto& p : pathInfo.parent) {
          std::cout << p.second.str() << " ";
        }
      }

      std::cout << "won: " << won << std::endl;
#endif
    }

    void left() { cursor.x = cursor.x == 0 ? cursor.x = map.size.x - 1 : cursor.x - 1; }

    void right() {
      cursor.x++;
      if (cursor.x >= map.size.x) {
        cursor.x = 0;
      }
    }

    void up() { cursor.y = cursor.y == 0 ? cursor.y = map.size.y - 1 : cursor.y - 1; }

    void down() {
      cursor.y++;
      if (cursor.y >= map.size.y) {
        cursor.y = 0;
      }
    }
  };
}

