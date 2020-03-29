#pragma once
#include "Screen.h"
#include "../resources/Tile.h"
#include "../resources/TilePack.h"
#include <SFML/Graphics.hpp>
#include <functional>

namespace StarPipe {
  class StartScreen : public Screen {
  public:
    const std::string path = "../PipeFlood/PipeFlood/assets/textures/";
    TileMap bg{ "splash.png" };
    bool start = false;
    
    StartScreen(GameInfo gameInfo, std::function<void()> keypressed) : Screen(gameInfo, keypressed) {}

    void key(sf::RenderWindow* window, sf::Event* event, StarPipe::InputInfo* input) override {
      keypressed();
      switch (event->key.code) {
      case sf::Keyboard::S: start = true; break;
      case sf::Keyboard::Q:
        window->close();
        break;
      }
    }

    void update(float delta) override {};
    void draw(sf::RenderWindow* window, float delta) override {
      window->clear(sf::Color::Black);
      window->draw(bg.sprite);
    }

    void mouse(sf::RenderWindow* window, sf::Event* event, StarPipe::InputInfo* input) override {};
    void create(sf::RenderWindow* window) override {
      sf::Vector2f targetSize(window->getSize().x, window->getSize().y);

      bg.sprite.setScale(
        targetSize.x / bg.sprite.getLocalBounds().width,
        targetSize.y / bg.sprite.getLocalBounds().height);
    };
    void resize(sf::RenderWindow* window) override {
      create(window);
    };

    void close(sf::RenderWindow* window) override {}
  };
}