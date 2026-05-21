#include "RandomMineGenerator.hpp"

#include <random>

RandomMineGenerator::RandomMineGenerator(CellGrid &grid, unsigned int mineCount)
    : MineGenerator{grid, mineCount}
{
}

void RandomMineGenerator::generate(int startCol, int startRow)
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

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            Cell *cell = grid.getCell(col, row);
            if (cell->isMine())
            {
                auto neighbors = grid.getNeighbors(col, row);
                // clang-format off
                // grid.forEachNeighbor(col, row, [](Cell *neighbor) {
                //     if (!neighbor->isMine()) {
                //         neighbor->setAdjacentMines(neighbor->getAdjacentMines() + 1);
                //     }
                // });
                // clang-format on
                for (auto const &neighbor : neighbors)
                {
                    if (!neighbor->isMine())
                    {
                        neighbor->setAdjacentMines(
                            neighbor->getAdjacentMines() + 1);
                    }
                }
            }
        }
    }
}

std::ostream &operator<<(std::ostream &os, RandomMineGenerator const &gen)
{
    os << "RandomMineGenerator[grid=" << gen.grid
       << ", mineCount=" << gen.mineCount << "]";
    return os;
}
