#pragma once
#include <cstdint>
#include <stdlib.h>
#include <time.h>

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

    bool inMargin(uint16_t x, uint16_t y, uint16_t lastX, uint16_t lastY) {
      return !(x > 0 && y > 0 && y < lastY && x < lastX);
    }

  }

}
