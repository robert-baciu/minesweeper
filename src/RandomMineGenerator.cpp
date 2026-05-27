#include "RandomMineGenerator.hpp"

#include <iostream>
#include <random>

RandomMineGenerator::RandomMineGenerator(PlayingGrid &grid,
                                         unsigned int mineCount)
    : MineGenerator(grid, mineCount)
{
}

void RandomMineGenerator::generateSafeStart(int startCol, int startRow)
{
    int cols = grid.getCols();
    int rows = grid.getRows();

    unsigned int placed = 0;
    while (placed < mineCount)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution distrib(0, cols * rows - 1);

        int randIndex = distrib(gen);
        int col = randIndex % cols;
        int row = randIndex / cols;

        if (col == startCol && row == startRow)
        {
            continue;
        }

        Cell *cell = grid.getCell(col, row);
        if (!cell->isMine())
        {
            cell->setMine();
            placed++;
        }
    }

    grid.all(
        [this](int col, int row)
        {
            Cell *cell = grid.getCell(col, row);
            if (!cell->isMine())
            {
                return;
            }

            // clang-format off
            grid.neighbors(col, row, [this](int neighborCol, int neighborRow) {
                Cell *neighbor = grid.getCell(neighborCol, neighborRow);
                if (!neighbor->isMine())
                {
                    neighbor->setAdjacentMines(neighbor->getAdjacentMines() + 1);
                }
            });
            // clang-format on
        });
}

std::ostream &operator<<(std::ostream &os, RandomMineGenerator const &gen)
{
    os << "RandomMineGenerator[mineCount=" << gen.mineCount << "]";
    return os;
}
