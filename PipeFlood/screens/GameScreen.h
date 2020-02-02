#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Map.h"
#include "../resources/Tile.h"
#include "Screen.h"
#include "../resources/TilePack.h"
#include "../animations/Animation.h"
#include "../GameTypes.h"

namespace PipeFlood {
  class GameScreen : public Screen {
  public:
    AnimationController<Sprite> controller;

    Audio ok{ "ok.wav" };
    Font font{ "Hack-Bold.ttf" };
    TileMap selector{ "selector.png" };
    TilePack tilePack{ GameInfo::TilePack };
    Sprites mapSprites;
    Sprites connectors;

    PipeFlood::PathInfo pathInfo;
    bool won = false;

  private:
    Map map;
    Vec2 cursor = Vec2{ 0, 0 };
    TraceInfo backtrace;

  public:
    GameScreen(GameInfo gameInfo, std::function<void()> keypressed) :
      Screen(gameInfo, keypressed),
      map{ gameInfo } {};

    void create(sf::RenderWindow* window) override {
      // Map background
      for (grid_t y = 0; y < gameInfo.mapSize.y+2; y++) {
        for (grid_t x = 0; x < gameInfo.mapSize.x+2; x++) {
          float pixelX = (x + gameInfo.margin) * gameInfo.tileSize.x;
          float pixelY = (y + gameInfo.margin) * gameInfo.tileSize.y;
          mapSprites.push_back(*tilePack.spriteGrid(x, y, gameInfo.mapSize.x + 1, gameInfo.mapSize.y + 1, sf::Vector2f{ pixelX, pixelY }));
        }
      }

      forMap([this](grid_t x, grid_t y, grid_t lastX, grid_t lastY, float pixelX, float pixelY) {
        auto sprite = map.getSprite(Vec2{ pixelX, pixelY }, Vec2{ x, y }, false);
        connectors.push_back(sprite);
      });

      animate();
    };

    void animate() {
      controller.clear();
      forMap([this](grid_t x, grid_t y, grid_t lastX, grid_t lastY, float pixelX, float pixelY) {
        auto index = (gameInfo.mapSize.x * y) + x;
        if (map.joined[x][y]) {
          if (map.field[x][y] == TileType::Start) {
            controller.push_back(std::make_unique<MoveAnimator>(connectors[index], Vec2{ 0, 4 }, 400));
          }
          else if (map.field[x][y] == TileType::End) {
            controller.push_back(std::make_unique<MoveAnimator>(connectors[index], Vec2{ 4, 0 }, 500));
          }
        }
      });
    }

    ~GameScreen() {
      mapSprites.clear();
      connectors.clear();
    }

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
      auto margin = gameInfo.margin + 1;
      if ((pos.x < gameInfo.tileSize.x * margin )
        || (pos.x > (gameInfo.tileSize.x * (gameInfo.mapSize.x + margin)))
        || (pos.y < gameInfo.tileSize.y * margin)
        || (pos.y > (gameInfo.tileSize.y * (gameInfo.mapSize.y + margin)))) {
        return;
      }
      cursor.x = pos.x / gameInfo.tileSize.x - margin;
      cursor.y = pos.y / gameInfo.tileSize.y - margin;
#ifdef DEBUG
      std::cout << cursor.x << ", " << cursor.y << std::endl;
#endif // DEBUG
      rotate(cursor);
      selector.sprite.setPosition(sf::Vector2f((float)((cursor.x + margin) * gameInfo.tileSize.x), (float)((cursor.y + margin) * gameInfo.tileSize.y)));
    }

    void update(float delta) override {
      for (const auto& c : controller) {
        c->update(delta);
      }
    };

    void resize(sf::RenderWindow* window) override {};

    void forMap(std::function<void(grid_t, grid_t, grid_t, grid_t, float, float)> callback) {
      for (uint16_t y = 0; y < gameInfo.mapSize.y; y++) {
        for (uint16_t x = 0; x < gameInfo.mapSize.x; x++) {
          float pixelX = (x + gameInfo.margin + 1) * gameInfo.tileSize.x;
          float pixelY = (y + gameInfo.margin + 1) * gameInfo.tileSize.y;
          callback(x, y, gameInfo.mapSize.x - 1, gameInfo.mapSize.y - 1, pixelX, pixelY);
        }
      }
    }

    void forScreen(std::function<void(grid_t, grid_t, grid_t, grid_t, float, float)> callback) {
      for (uint16_t y = 0; y < gameInfo.screenSize.y; y++) {
        for (uint16_t x = 0; x < gameInfo.screenSize.x; x++) {
          float pixelX = x * gameInfo.tileSize.x;
          float pixelY = y * gameInfo.tileSize.y;
          callback(x, y, gameInfo.screenSize.x - 1, gameInfo.screenSize.y - 1, pixelX, pixelY);
        }
      }
    }

    void draw(sf::RenderWindow* window, float delta) override {
      window->clear(sf::Color::Black);

      forScreen([this, window](grid_t x, grid_t y, grid_t lastX, grid_t lastY, float pixelX, float pixelY) {
        tilePack.bg.sprite.setPosition(sf::Vector2f(pixelX, pixelY));
        window->draw(tilePack.bg.sprite);
      });

      for (grid_t y = 0; y < (gameInfo.mapSize.y+2); y++) {
        for (grid_t x = 0; x < (gameInfo.mapSize.x+2); x++) {
          auto index = ((gameInfo.mapSize.x + 2) * y) + x;
          window->draw(mapSprites[index]);
        }
      }

      forMap([this, window](grid_t x, grid_t y, grid_t lastX, grid_t lastY, float pixelX, float pixelY) {
        auto index = (gameInfo.mapSize.x * y) + x;
        if (won && backtrace.visited[Vec2{ x, y }.str()]) {
          connectors[index].setColor(sf::Color{ 200, 100, 100 });
        }
        window->draw(connectors[index]);
      });

      window->draw(selector.sprite);

      auto text = font.text(std::to_string(1 / delta), 30);
      text.setPosition(sf::Vector2f{ (float)gameInfo.tileSize.x, (float)gameInfo.tileSize.y / 4 });
      window->draw(text);
    }

    void close(sf::RenderWindow* window) override {}

  private:
    void rotate(Vec2 pos) {
      auto type = map.field[pos.x][pos.y];
      if (type == TileType::None || type == TileType::None2 || type == TileType::Void) {
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
      pathInfo = map.BFS(Vec2{ 0, 0 }, map.target);
      if (!won && pathInfo.visited[map.target.str()]) {
        backtrace = map.backTraceFrom(pathInfo, map.target, Vec2{ 0, 0 });
        won = true;
      }

      forMap([this](grid_t x, grid_t y, grid_t lastX, grid_t lastY, float pixelX, float pixelY) {
        auto index = (gameInfo.mapSize.x * y) + x;
        connectors[index] = map.getSprite(Vec2{ pixelX, pixelY }, Vec2{ x, y }, pathInfo.visited[Vec2{ x,y }.str()]);
      });

      //ok.play();

      animate();

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

    void left() { cursor.x = cursor.x == 0 ? cursor.x = gameInfo.mapSize.x - 1 : cursor.x - 1; }

    void right() {
      cursor.x++;
      if (cursor.x >= gameInfo.mapSize.x) {
        cursor.x = 0;
      }
    }

    void up() { cursor.y = cursor.y == 0 ? cursor.y = gameInfo.mapSize.y - 1 : cursor.y - 1; }

    void down() {
      cursor.y++;
      if (cursor.y >= gameInfo.mapSize.y) {
        cursor.y = 0;
      }
    }
    };
  }

