#include "Grid.hpp"

Grid::Grid(int cols, int rows) : cols(cols), rows(rows)
{
    cells.resize(cols * rows);
}

void Grid::placeRandMines(int startCol, int startRow, unsigned int mineCount)
{
    unsigned int placedCount = 0;
    while (placedCount < mineCount)
    {
        // TODO: replace with C++11 random library
        int col = rand() % cols;
        int row = rand() % rows;

        // TODO: avoid mines *around* the first revealed cell
        if (startCol == col && startRow == row)
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

    for (int col = 0; col < cols; col++)
    {
        for (int row = 0; row < rows; row++)
        {
            if (getCell(col, row)->getType() == Cell::Type::Mine)
            {
                auto neighbors = getCellNeighbors(col, row);
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

std::vector<Cell *> Grid::getCellNeighbors(int col, int row)
{
    std::vector<Cell *> neighbors;
    neighbors.reserve(8);

    for (int colOff = -1; colOff <= 1; colOff++)
    {
        for (int rowOff = -1; rowOff <= 1; rowOff++)
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

std::vector<Cell> const &Grid::getCells() const
{
    return cells;
}

std::ostream &operator<<(std::ostream &os, Grid const &grid)
{
    os << "Grid[cols=" << grid.cols << ", rows=" << grid.rows << "]";
    return os;
}
