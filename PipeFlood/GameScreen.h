#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.h"
#include "Tile.h"
#include "Screen.h"
#include "TilePack.h"
#include "Animation.h"

namespace PipeFlood {
  typedef std::vector<Sprite> Sprites;

  class GameScreen : public Screen {
  public:
    AnimationController<Sprite> controller;

    TileMap selector{ "selector.png" };
    TilePack tilePack{ 1 };
    Sprites mapSprites;
    Sprites connectors;

    PipeFlood::PathInfo pathInfo;
    bool won = false;

  private:
    Map map;  
    v2 cursor = v2{ 0, 0 };
    TraceInfo backtrace;

  public:
    GameScreen(GameInfo gameInfo, std::function<void()> keypressed) :
      Screen(gameInfo, keypressed),
      map{ gameInfo } {};

    void create(sf::RenderWindow* window) override {
      traverse([this](uint16_t x, uint16_t y, uint16_t lastX, uint16_t lastY, float pixelX, float pixelY) {
        const auto v = v2{ x, y };
        mapSprites.push_back(*tilePack.spriteGrid(x, y, lastX, lastY, sf::Vector2f{ pixelX, pixelY }));
        auto sprite = map.getSprite(v, false);
        connectors.push_back(sprite);
      });
      for (uint16_t y = 0; y < gameInfo.boardSize.y; y++) {
        for (uint16_t x = 0; x < gameInfo.boardSize.x; x++) {
          auto index = (gameInfo.boardSize.x * y) + x;
          if (map.field[x][y] == TileType::Start) {
            controller.push_back(std::make_unique<ScaleAnimator>(connectors[index], 0.01, 0.06));
          }
          else if (map.field[x][y] == TileType::End) {
            controller.push_back(std::make_unique<MoveAnimator>(connectors[index], v2{ 5, 0 }, 500));
          }
        }
      }
    };

    void key(sf::RenderWindow* window, sf::Event* event, PipeFlood::InputInfo* input) override {
      switch (event->key.code) {
      case sf::Keyboard::Enter: rotate(cursor); break;
      case sf::Keyboard::Right: right(); break;
      case sf::Keyboard::Up: up(); break;
      case sf::Keyboard::Down: down(); break;
      case sf::Keyboard::Left: left(); break;
      }
      selector.sprite.setPosition(sf::Vector2f(cursor.x * gameInfo.tileSize.x, cursor.y * gameInfo.tileSize.y));
    }

    void mouse(sf::RenderWindow* window, sf::Event* event, PipeFlood::InputInfo* input) override {
      auto pos = sf::Mouse::getPosition(*window);
      cursor.x = pos.x / gameInfo.tileSize.x;
      cursor.y = pos.y / gameInfo.tileSize.y;
      rotate(cursor);
      selector.sprite.setPosition(sf::Vector2f( (float)(cursor.x * gameInfo.tileSize.x), (float)(cursor.y * gameInfo.tileSize.y)));
    }

    void update(float delta) override {
      for (const auto& c : controller) {
        c->update(delta);
      }
    };

    void resize(sf::RenderWindow* window) override {};

    inline void traverse(std::function<void(uint16_t, uint16_t, uint16_t, uint16_t, float, float)> callback) {
      for (uint16_t y = 0; y < gameInfo.boardSize.y; y++) {
        for (uint16_t x = 0; x < gameInfo.boardSize.x; x++) {
          float pixelX = x * gameInfo.tileSize.x;
          float pixelY = y * gameInfo.tileSize.y;
          callback(x, y, gameInfo.boardSize.x - 1, gameInfo.boardSize.y - 1, pixelX, pixelY);
        }
      }
    }

    void draw(sf::RenderWindow* window, float delta) override {
      window->clear(sf::Color::Black);

      traverse([this, window](uint16_t x, uint16_t y, uint16_t lastX, uint16_t lastY, float pixelX, float pixelY) {
        tilePack.bg.sprite.setPosition(sf::Vector2f(pixelX, pixelY));
        window->draw(tilePack.bg.sprite);
        auto index = (gameInfo.boardSize.x * y) + x;
        if (won && backtrace.visited[v2{ x, y }.str()]) {
          connectors[index].setColor(sf::Color{ 200, 100, 100 });
        }
        window->draw(mapSprites[index]);
        window->draw(connectors[index]);
      });

      window->draw(selector.sprite);
    }

    void close(sf::RenderWindow* window) override {}

  private:
    void rotate(v2 pos) {
      if (map.field[pos.x][pos.y] == TileType::None) {
        return;
      }
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

      for (uint16_t y = 0; y < gameInfo.boardSize.y; y++) {
        for (uint16_t x = 0; x < gameInfo.boardSize.x; x++) {
          auto index = (gameInfo.boardSize.x * y) + x;
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

    void left() { cursor.x = cursor.x == 0 ? cursor.x = gameInfo.boardSize.x - 1 : cursor.x - 1; }

    void right() {
      cursor.x++;
      if (cursor.x >= gameInfo.boardSize.x) {
        cursor.x = 0;
      }
    }

    void up() { cursor.y = cursor.y == 0 ? cursor.y = gameInfo.boardSize.y - 1 : cursor.y - 1; }

    void down() {
      cursor.y++;
      if (cursor.y >= gameInfo.boardSize.y) {
        cursor.y = 0;
      }
    }
  };
}

