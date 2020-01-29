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
  }
}
