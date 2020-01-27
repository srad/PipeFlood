#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>;
#include "GameTypes.h";
#include "PipeFlood.h";
#include "Tile.h"

namespace PipeFlood {

  class Game {
  public:
    v2 resolution;
    PipeFlood::Tiles::Tile selector = PipeFlood::Tiles::Tile{ "selector.png" };
    PipeFlood::PathInfo pathInfo;
    bool won = false;

    Game(v2 size) : map{ size }, resolution{ static_cast<uint16_t>(size.x * 64), static_cast<uint16_t>(size.y * 64) } {};

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

      map.rotation[cursor.x][cursor.y] = (map.rotation[cursor.x][cursor.y] + 1) % 4;

#ifdef DEBUG
      std::cout << cursor.str() << " " << map.rotation[cursor.x][cursor.y] << std::endl;
#endif

      map.checkJoins();

#ifdef DEBUG
      map.printJoins();
#endif

      pathInfo = map.BFS(v2{ 0,0 }, map.target);
      won = pathInfo.visited[map.target.str()];

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

      selector.sprite.setPosition(sf::Vector2f(cursor.x * map.tileResized.x, cursor.y * map.tileResized.y));
    }

    void draw(sf::RenderWindow* window) {
      window->clear(sf::Color::Black);

      for (uint16_t x = 0; x < map.size.x; x++) {
        for (uint16_t y = 0; y < map.size.y; y++) {
          window->draw(map.getSprite(v2{ x, y }, pathInfo.visited[v2{x,y}.str()]));
        }
      }
      window->draw(selector.sprite);

      // Draw path to goal
      if (won) {
        auto startField = map.getSprite(v2{ 0,0 });
        startField.setColor(sf::Color{ 150, 200, 100 });
        window->draw(startField);
        
        auto it = pathInfo.parent.find(map.target.str());
        auto end = pathInfo.parent.find(v2{ 0,0 }.str());
        while (it->second.x != 0 && it->second.y != 0) {
          auto sprite = map.getSprite(it->second);
          sprite.setColor(sf::Color{ 150, 200, 100 });
          window->draw(sprite);
          it = pathInfo.parent.find(it->second.str());
        }

        auto targetField = map.getSprite(map.target);
        targetField.setColor(sf::Color{ 150, 200, 100 });
        window->draw(targetField);

      }
    }

    void create(sf::RenderWindow* window) {}
    void resize(sf::RenderWindow* window) {}

  private:
    Map map;
    v2 cursor;

    void rotate(v2 pos) { map.rotation[pos.x][pos.y] = (map.rotation[pos.x][pos.y] + 1) % 4; }
    
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

