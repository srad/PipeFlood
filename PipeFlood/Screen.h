#pragma once
#include "GameTypes.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <functional>

namespace PipeFlood {
  class Screen {
  public:
    v2 size;
    std::function<void()> keypressed;
    Screen(PipeFlood::v2 size, std::function<void()> keypressed) : size{ size }, keypressed{ keypressed } {};
    bool doUpdate = true;
    bool doDraw = true;

  public:
    virtual void mouse(sf::RenderWindow* window, sf::Event* event, PipeFlood::InputInfo* input) = 0;
    virtual void key(sf::RenderWindow* window, sf::Event* event, PipeFlood::InputInfo* input) = 0;
    virtual void update(float delta) = 0;
    virtual void draw(sf::RenderWindow* window, float delta) = 0;
    virtual void create(sf::RenderWindow* window) = 0;
    virtual void close(sf::RenderWindow* window) = 0;
    virtual void resize(sf::RenderWindow* window) = 0;
  };
}