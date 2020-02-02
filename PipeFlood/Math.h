#pragma once
#include <cstdint>
#include <stdlib.h>
#include <time.h>
#include <random>
#include "GameTypes.h"

namespace PipeFlood {

  namespace Math {

    int rnd(int min, int max) {
      static bool first = true;
      if (first) {
        srand(time(NULL));
        first = false;
      }
      return min + rand() % ((max + 1) - min);
    }

    bool inMargin(grid_t x, grid_t y, grid_t lastX, grid_t lastY) {
      return !(x > 0 && y > 0 && y < lastY && x < lastX);
    }

    // Source: https://github.com/SFML/SFML/wiki/Source:-Particle-System
    class Randomizer {
    public:
      Randomizer() : device_(), engine_(device_()) {};
      int rnd(int a, int b) {
        std::uniform_int_distribution<int> uni_dist(a, b);
        return uni_dist(engine_);
      };
      double rnd(double a, double b) {
        std::uniform_real_distribution<double> uni_dist(a, b);
        return uni_dist(engine_);
      };
    private:
      std::random_device device_;
      std::default_random_engine engine_;
    };

  }

}
