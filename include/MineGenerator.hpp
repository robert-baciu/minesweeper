#pragma once

#include "CellGrid.hpp"

class MineGenerator
{
  public:
    explicit MineGenerator(unsigned int mineCount);

    virtual ~MineGenerator() = default;

    virtual void generate(CellGrid &grid, int startCol, int startRow) = 0;

  protected:
    unsigned int mineCount;
};
