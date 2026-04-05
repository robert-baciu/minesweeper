#include "CellGrid.hpp"

#include <random>

CellGrid::CellGrid(int cols, int rows)
    : cols(cols), rows(rows), cells(cols * rows)
{
}

std::vector<Cell> const &CellGrid::getCells() const
{
    return cells;
}

std::vector<Cell *> CellGrid::getNeighbors(int col, int row)
{
    std::vector<Cell *> neighbors;
    neighbors.reserve(8);

    for (int rowOff = -1; rowOff <= 1; rowOff++)
    {
        for (int colOff = -1; colOff <= 1; colOff++)
        {
            if (colOff == 0 && rowOff == 0)
            {
                continue;
            }

            if (Cell *neighbor = getCell(col + colOff, row + rowOff))
            {
                neighbors.push_back(neighbor);
            }
        }
    }

    return neighbors;
}

std::ostream &operator<<(std::ostream &os, CellGrid const &grid)
{
    os << "Grid[cols=" << grid.cols << ", rows=" << grid.rows << "]";
    return os;
}

void CellGrid::generateMines(int startCol, int startRow, unsigned int mineCount,
                             unsigned int minDist /* = 2 */)
{
    unsigned int placedCount = 0;
    while (placedCount < mineCount)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, cols * rows - 1);

        int randIndex = distrib(gen);
        int col = randIndex % cols;
        int row = randIndex / cols;

        auto deltaCol = static_cast<unsigned int>(std::abs(startCol - col));
        auto deltaRow = static_cast<unsigned int>(std::abs(startRow - row));
        if (deltaCol < minDist && deltaRow < minDist)
        {
            continue;
        }

        Cell *cell = getCell(col, row);
        if (cell->getType() == Cell::Type::Empty)
        {
            cell->setType(Cell::Type::Mine);
            placedCount++;
        }
    }

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (Cell *cell = getCell(col, row);
                cell->getType() == Cell::Type::Mine)
            {
                auto neighbors = getNeighbors(col, row);
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
