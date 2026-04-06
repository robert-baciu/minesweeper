#pragma once

#include "MineGenerator.hpp"

class RandomMineGenerator : public MineGenerator
{
  public:
    RandomMineGenerator(unsigned int mineCount, unsigned int safeDistance);

    void generate(CellGrid &grid, int startCol, int startRow) override;
};
