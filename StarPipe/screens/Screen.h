#pragma once
#include "../GameTypes.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <functional>

namespace StarPipe {
  class Screen {
  public:
    GameInfo gameInfo;
    std::function<void()> keypressed;
    Screen(GameInfo gameInfo, std::function<void()> keypressed) : gameInfo{ gameInfo }, keypressed{ keypressed } {};
    bool doUpdate = true;
    bool doDraw = true;

  public:
    virtual void mouse(sf::RenderWindow* window, sf::Event* event, StarPipe::InputInfo* input) = 0;
    virtual void key(sf::RenderWindow* window, sf::Event* event, StarPipe::InputInfo* input) = 0;
    virtual void update(float delta) = 0;
    virtual void draw(sf::RenderWindow* window, float delta) = 0;
    virtual void create(sf::RenderWindow* window) = 0;
    virtual void close(sf::RenderWindow* window) = 0;
    virtual void resize(sf::RenderWindow* window) = 0;
  };
}