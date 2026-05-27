#pragma once

#include "PlayingGrid.hpp"

class MineGenerator
{
  public:
    MineGenerator(PlayingGrid &grid, unsigned int mineCount)
        : grid(grid),
          mineCount(mineCount)
    {
    }

    virtual ~MineGenerator() = default;

    virtual void generateSafeStart(int startCol, int startRow) = 0;

  protected:
    PlayingGrid &grid;

    unsigned int const mineCount;
};
