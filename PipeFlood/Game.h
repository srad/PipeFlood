#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h";
#include "GameScreen.h";
#include "StartScreen.h"

namespace PipeFlood {

  struct EngineConfig {
    uint16_t fps;
    bool vSync;
  };
  
  class Game {
  public:
    Screen* screen;
    GameScreen gameScreen{ v2{15, 12}, [this] {} };
    StartScreen startScreen{ v2{15, 12}, [this] {
      screen = &gameScreen;
    } };
    InputInfo inputInfo;
    sf::RenderWindow window{ sf::VideoMode(gameScreen.resolution.x, gameScreen.resolution.y), "PipeFlood" };

    Game() {
      screen = &startScreen;
      Game(EngineConfig{ 30, false });
    }

    Game(const EngineConfig config) {
      window.setFramerateLimit(config.fps);
      window.setVerticalSyncEnabled(config.vSync);
    }

    void start() {
      screen->create(&window);
      while (window.isOpen()) {
        sf::Event event;
        bool action;

        while (window.hasFocus() && window.pollEvent(event)) {
          // EVENT
          action = true;
          switch (event.type) {
          case sf::Event::Resized: screen->resize(&window); break;
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
              screen->key(&window, &event, &inputInfo);
            }

            // MOUSE
            if (!inputInfo.mouseDown && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
              inputInfo.mouseDown = true;
              screen->mouse(&window, &event, &inputInfo);
            }
          }
        }

        screen->update();
        screen->draw(&window);
        window.display();
      }
    }
  };

}