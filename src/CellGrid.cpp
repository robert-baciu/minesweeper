#include "CellGrid.hpp"

CellGrid::CellGrid(int cols, int rows)
    : cols(cols), rows(rows), cells(cols * rows)
{
}

int CellGrid::getCols() const
{
    return cols;
}

int CellGrid::getRows() const
{
    return rows;
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
