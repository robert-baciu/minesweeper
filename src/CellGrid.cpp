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

Cell *CellGrid::getCell(int col, int row)
{
    return isInBounds(col, row) ? &cells[col + row * cols] : nullptr;
}

Cell const *CellGrid::getCell(int col, int row) const
{
    return isInBounds(col, row) ? &cells[col + row * cols] : nullptr;
}

std::vector<Cell> const &CellGrid::getCells() const
{
    return cells;
}

// void CellGrid::forEachNeighbor(int col, int row,
//                                std::function<void(Cell *)> const &callback)
// {
//     for (int rowOff = -1; rowOff <= 1; rowOff++)
//     {
//         for (int colOff = -1; colOff <= 1; colOff++)
//         {
//             if (colOff == 0 && rowOff == 0)
//             {
//                 continue;
//             }
//
//             if (Cell *neighbor = getCell(col + colOff, row + rowOff))
//             {
//                 callback(neighbor);
//             }
//         }
//     }
// }

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

bool CellGrid::isInBounds(int col, int row) const
{
    return col >= 0 && col < cols && row >= 0 && row < rows;
}
