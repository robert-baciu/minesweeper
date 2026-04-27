#pragma once

#include "CellGrid.hpp"

class MineGenerator
{
  public:
    MineGenerator(CellGrid &grid, unsigned int mineCount)
        : grid(grid), mineCount(mineCount)
    {
    }

    virtual ~MineGenerator() = default;

    virtual void generate(int startCol, int startRow) = 0;

  protected:
    CellGrid &grid;

    unsigned int mineCount;
};
