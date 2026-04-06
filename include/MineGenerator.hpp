#pragma once

#include "CellGrid.hpp"

class MineGenerator
{
  public:
    MineGenerator(unsigned int mineCount, unsigned int safeDistance);

    virtual ~MineGenerator() = default;

    virtual void generate(CellGrid &grid, int startCol, int startRow) = 0;

  protected:
    unsigned int mineCount;
    unsigned int safeDistance;
};
