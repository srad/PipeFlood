#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>;
#include "GameTypes.h";
#include "PipeFlood.h";

namespace PipeFlood {

  struct Game {
    v2 resolution;
    Tile selector;

  public:
    Game(v2 size) : selector{ Tile{"selector.png"} }, map{ size }, resolution{ static_cast<uint16_t>(size.x * 64), static_cast<uint16_t>(size.y * 64) } {};

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
      std::cout << cursor.str() << " " << map.rotation[cursor.x][cursor.y] << std::endl;

      selector.sprite.setPosition(sf::Vector2f(cursor.x * map.tileResized.x, cursor.y * map.tileResized.y));
    }

    void draw(sf::RenderWindow* window) {
      window->clear(sf::Color::Black);

      for (uint16_t x = 0; x < map.size.x; x++) {
        for (uint16_t y = 0; y < map.size.y; y++) {
          auto sprite = map.getSprite(v2{ x, y });
          window->draw(sprite);
        }
      }
      window->draw(selector.sprite);
    }

  private:
    Map map;
    v2 cursor;

    void rotate(v2 pos) {
      map.rotation[pos.x][pos.y] = (map.rotation[pos.x][pos.y] + 1) % 4;
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

