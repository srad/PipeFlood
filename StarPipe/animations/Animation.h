#pragma once
#include "../GameTypes.h"
#include <SFML\System\Clock.hpp>

namespace StarPipe {

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

  struct MoveAnimator : public Animator<Sprite> {
  private:
    sf::Clock animClock;
    enum class AnimState { Start, Up, Down };
    AnimState state = AnimState::Start;
    Vec2 offset;
    float milliseconds;

  public:
    MoveAnimator(Sprite& sprite, Vec2 offset, Milliseconds milliseconds) : offset{ offset }, milliseconds{ milliseconds }, Animator{ sprite } {}

    void update(float delta) override {
      float moveY = 0;
      float moveX = 0;
      auto dT = animClock.getElapsedTime().asMilliseconds();

      if (dT > milliseconds) {
        switch (state) {
        case AnimState::Start:
          state = AnimState::Up;
          animClock.restart();
          moveY = -offset.y;
          moveX = -offset.x;
          break;
        case AnimState::Up:
          state = AnimState::Start;
          animClock.restart();
          moveY = offset.y;
          moveX = offset.x;
          break;
        }
      }

      entity.move(sf::Vector2f{ moveX, moveY });
    }
  };
  
  struct ScaleAnimator : public Animator<Sprite> {
  private:
    enum class AnimState { Up, Down };
    AnimState state = AnimState::Up;
    float growth;
    float current = 0;
    float max;

  public:
    ScaleAnimator(Sprite& sprite, float growth, float max) : growth{ growth }, max{ max }, Animator{ sprite } {}

    void update(float delta) override {
      float scale = 1.0;
      switch (state) {
      case AnimState::Up:
        if (current < max) {
          current += growth;
          scale = 1 + growth;
        }
        else {
          state = AnimState::Down;
        }
        break;
      case AnimState::Down:
        if (current > 0) {
          current -= growth;
          scale = 1 - growth;
        }
        else {
          state = AnimState::Up;
        }
        break;
      }
      entity.scale(sf::Vector2f{ scale, scale });
    }
  };

}
