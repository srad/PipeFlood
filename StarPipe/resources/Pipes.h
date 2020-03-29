#pragma once
#include <vector>

namespace StarPipe {
  namespace Pipes {
    // Each vector defines the each side of the square 
    // in the order given by the enum "side". Only 3 bits for each side is needed,
    // so this declaration could be more compact, but meh.

    const bool pipe = true;
    const bool none = false;

    const std::vector<std::vector<std::vector<bool>>> vPipeMask
    {
      // From left to right each element with a rotation clockwise each step by pi/2

      // Endpoint
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   |   |   | |   |   |   | |   | X |   | |   |   |   |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   | X |   | | X | X |   | |   | X |   | |   | X | X |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   | X |   | |   |   |   | |   |   |   | |   |   |   |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      std::vector<std::vector<bool>>
      {
        { none, none, none, pipe },
        { pipe, none, none, none },
        { none, none, pipe, none },
        { none, pipe, none, none },
      },

      // Opening
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   |   |   | |   |   |   | |   | X |   | |   |   |   |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   | X |   | | X | X |   | |   | X |   | |   | X | X |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   | X |   | |   |   |   | |   |   |   | |   |   |   |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      std::vector<std::vector<bool>>
      {
        { none, none, none, pipe },
        { pipe, none, none, none },
        { none, none, pipe, none },
        { none, pipe, none, none },
      },

      // +---+---+---+
      // |   | X |   |
      // +---+---+---+
      // | X | X | X |
      // +---+---+---+
      // |   | X |   |
      // +---+---+---+
      /*
      std::vector<std::vector<bool>>
      {
        {pipe, pipe, pipe, pipe },
        {pipe, pipe, pipe, pipe },
        {pipe, pipe, pipe, pipe },
        {pipe, pipe, pipe, pipe }
      },
      */

      // I pipe
      // +---+---+---+ +---+---+---+
      // |   | X |   | |   |   |   |
      // +---+---+---+ +---+---+---+
      // |   | X |   | | X | X | X |
      // +---+---+---+ +---+---+---+
      // |   | X |   | |   |   |   |
      // +---+---+---+ +---+---+---+
      std::vector<std::vector<bool>>
      {
        {none, none, pipe, pipe },
        {pipe, pipe, none, none },
        {none, none, pipe, pipe },
        {pipe, pipe, none, none }
      },

      // T pipe
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   |   |   | |   | X |   | |   | X |   | |   | X |   |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // | X | X | X | | X | X |   | | X | X | X | |   | X | X |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   | X |   | |   | X |   | |   |   |   | |   | X |   |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      std::vector<std::vector<bool>>
      {
        { pipe, pipe, none, pipe },
        { pipe, none, pipe, pipe },
        { pipe, pipe, pipe, none },
        { none, pipe, pipe, pipe },
      },

      // Edge pipe
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   |   |   | |   |   |   | |   | X |   | |   | X |   |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   | X | X | | X | X |   | | X | X |   | |   | X | X |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      // |   | X |   | |   | X |   | |   |   |   | |   |   |   |
      // +---+---+---+ +---+---+---+ +---+---+---+ +---+---+---+
      std::vector<std::vector<bool>>
      {
        { none, pipe, none, pipe },
        { pipe, none, none, pipe },
        { pipe, none, pipe, none },
        { none, pipe, pipe, none },
      },

      // No connection
      // +---+---+---+
      // |   |   |   |
      // +---+---+---+
      // |   | X |   |
      // +---+---+---+
      // |   |   |   |
      // +---+---+---+
      std::vector<std::vector<bool>>
      {
        { none, none, none, none },
        { none, none, none, none },
        { none, none, none, none },
        { none, none, none, none },
      },

      // No connection
      // +---+---+---+
      // |   |   |   |
      // +---+---+---+
      // |   | X |   |
      // +---+---+---+
      // |   |   |   |
      // +---+---+---+
      std::vector<std::vector<bool>>
      {
        { none, none, none, none },
        { none, none, none, none },
        { none, none, none, none },
        { none, none, none, none },
      },

      // No connection
      // +---+---+---+
      // |   |   |   |
      // +---+---+---+
      // |   | X |   |
      // +---+---+---+
      // |   |   |   |
      // +---+---+---+
      std::vector<std::vector<bool>>
      {
        { none, none, none, none },
        { none, none, none, none },
        { none, none, none, none },
        { none, none, none, none },
      }
    };
  }
}