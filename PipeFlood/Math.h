#pragma once
#include <cstdint>
#include <stdlib.h>

namespace PipeFlood {

  namespace Math {

    inline uint16_t rnd(uint16_t min, uint16_t max) {
      return rand() % (max - min + 1) + min;
    }

  }

}
