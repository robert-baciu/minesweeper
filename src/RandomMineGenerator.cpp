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
        if (cell->getType() == Cell::Type::Empty)
        {
            cell->setType(Cell::Type::Mine);
            placed++;
        }
    }

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (Cell *cell = grid.getCell(col, row);
                cell->getType() == Cell::Type::Mine)
            {
                auto neighbors = grid.getNeighbors(col, row);
                for (auto const &neighbor : neighbors)
                {
                    if (neighbor->getType() == Cell::Type::Empty)
                    {
                        neighbor->setMineCount(neighbor->getMineCount() + 1);
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
