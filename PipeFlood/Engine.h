#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

namespace PipeFlood {

  struct EngineConfig {
    uint16_t fps;
    bool vSync;
  };
  
  class Engine {
  public:
    Game game{ v2{15, 12} };
    InputInfo inputInfo;
    sf::RenderWindow window{ sf::VideoMode(game.resolution.x, game.resolution.y), "PipeFlood" };

    Engine() {
      Engine(EngineConfig{ 30, false });
    }

    Engine(const EngineConfig config) {
      window.setFramerateLimit(config.fps);
      window.setVerticalSyncEnabled(config.vSync);
    }

    void start() {
      game.create(&window);
      while (window.isOpen()) {
        sf::Event event;
        bool action;

        while (window.hasFocus() && window.pollEvent(event)) {
          // EVENT
          action = true;
          switch (event.type) {
          case sf::Event::Resized: game.resize(&window); break;
          case sf::Event::Closed: window.close(); break;
          case sf::Event::KeyReleased: inputInfo.keyDown = false; break;
          case sf::Event::MouseButtonPressed: action = true; break;
          case sf::Event::MouseButtonReleased: inputInfo.mouseDown = false; break;
          default: action = false; break;
          }

          if (action) {
            // INPUT
            if (!inputInfo.keyDown && sf::Event::KeyPressed) {
              inputInfo.keyDown = true;
              game.key(&window, &event, &inputInfo);
            }

            // MOUSE
            if (!inputInfo.mouseDown && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
              inputInfo.mouseDown = true;
              game.mouse(&window, &event, &inputInfo);
            }
          }
        }

        game.draw(&window);
        window.display();
      }
    }
  };

}