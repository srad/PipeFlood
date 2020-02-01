#pragma once
#include "GameTypes.h"
#include <SFML\System\Clock.hpp>

namespace PipeFlood {

  template <typename T>
  struct Animator {
    T& entity;
    virtual void update(float delta) = 0;
    Animator(T& entity) : entity{ entity } {};
  };

  template<typename T>
  struct AnimationController : public std::vector<std::unique_ptr<Animator<T>>> {};
  
  typedef float Milliseconds;
  typedef float Distance;

  struct UpDownAnimator : public Animator<Sprite> {
    sf::Clock animClock;
    enum class AnimState { Start, Up, Down };
    AnimState state = AnimState::Start;
    float offset, milliseconds;

    UpDownAnimator(Sprite& sprite, Distance offset, Milliseconds milliseconds) : offset{ offset }, milliseconds{ milliseconds }, Animator{ sprite } {}

    void update(float delta) override {
      float moveY = 0;
      auto dT = animClock.getElapsedTime().asMilliseconds();

      if (dT > milliseconds) {
        switch (state) {
        case AnimState::Start:
          state = AnimState::Up;
          animClock.restart();
          moveY = -offset;
          break;
        case AnimState::Up:
          state = AnimState::Start;
          animClock.restart();
          moveY = offset;
          break;
        }
      }

      entity.move(sf::Vector2f{ 0, moveY });
    }
  };

}
