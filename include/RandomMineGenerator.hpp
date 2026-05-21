#pragma once

#include "MineGenerator.hpp"

class RandomMineGenerator : public MineGenerator
{
  public:
    RandomMineGenerator(CellGrid &grid, unsigned int mineCount);

    void generateSafeStart(int startCol, int startRow) override;

    friend std::ostream &operator<<(std::ostream &os,
                                    RandomMineGenerator const &gen);
};
