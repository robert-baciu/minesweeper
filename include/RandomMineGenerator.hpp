#pragma once

#include "MineGenerator.hpp"

class RandomMineGenerator : public MineGenerator
{
  public:
    explicit RandomMineGenerator(unsigned int mineCount);

    void generate(CellGrid &grid, int startCol, int startRow) override;
};
