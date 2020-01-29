#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "GameScreen.h"
#include "StartScreen.h"
#define DEBUG

namespace PipeFlood {
  class Game {
  private:
    Screen* screen;
    GameScreen gameScreen{ v2{15, 12}, [this] {} };
    StartScreen startScreen{ v2{15, 12}, [this] { setScreen(&gameScreen); } };

    InputInfo inputInfo;
    sf::RenderWindow window{ sf::VideoMode(gameScreen.resolution.x, gameScreen.resolution.y), "PipeFlood", sf::Style::Titlebar | sf::Style::Close };

    sf::Clock clock;

  public:
    Game() {
      screen = &startScreen;
      window.setFramerateLimit(30);
      window.setVerticalSyncEnabled(false);
    }

    void start() {
      screen->create(&window);

      // A pressed key or button calls only *once* the callback
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

        // CALLBACKS
        float delta = clock.restart().asSeconds();
        if (screen->doUpdate) {
          screen->update(delta);
        }
        if (screen->doDraw) {
          screen->draw(&window, delta);
        }

        window.display();
      }
    }

    void setScreen(Screen* screen) {
      this->screen->close(&window);
      this->screen = screen;
      this->screen->create(&window);
    }
  };

}