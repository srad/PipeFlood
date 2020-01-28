#pragma once
#include "Screen.h";
#include "Tile.h";
#include <SFML/Graphics.hpp>
#include <functional>

namespace PipeFlood {
  class StartScreen : public Screen {
  public:
    const std::string path = "C:/Users/saman/src/PipeFlood/PipeFlood/assets/textures/";
    TileMap bg{ "splash.png" };
    bool start = false;
    
    StartScreen(v2 size, std::function<void()> keypressed) : Screen(size, keypressed) {}

    void key(sf::RenderWindow* window, sf::Event* event, PipeFlood::InputInfo* input) {
      keypressed();
      switch (event->key.code) {
      case sf::Keyboard::S: start = true; break;
      case sf::Keyboard::Q:
        window->close();
        break;
      }
    }

    void draw(sf::RenderWindow* window) {
      window->clear(sf::Color::Black);
      window->draw(bg.sprite);
    }

    void mouse(sf::RenderWindow* window, sf::Event* event, PipeFlood::InputInfo* input) {};
    void update() {};
    void create(sf::RenderWindow* window) {};
    void resize(sf::RenderWindow* window) {};
  };
}