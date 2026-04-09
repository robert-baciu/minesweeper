#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Cell.hpp"

class CellGrid
{
  public:
    CellGrid(int cols, int rows);

    int getCols() const;
    int getRows() const;

    inline Cell *getCell(int col, int row);
    inline Cell const *getCell(int col, int row) const;
    std::vector<Cell> const &getCells() const;

    // TODO: forEachNeighbor
    std::vector<Cell *> getNeighbors(int col, int row);

    inline bool isInBounds(int col, int row) const;

    friend std::ostream &operator<<(std::ostream &os, CellGrid const &grid);

  private:
    int const cols;
    int const rows;
    std::vector<Cell> cells;
};

inline Cell *CellGrid::getCell(int col, int row)
{
    return isInBounds(col, row) ? &cells[col + row * cols] : nullptr;
}

inline Cell const *CellGrid::getCell(int col, int row) const
{
    return isInBounds(col, row) ? &cells[col + row * cols] : nullptr;
}

inline bool CellGrid::isInBounds(int col, int row) const
{
    return col >= 0 && col < cols && row >= 0 && row < rows;
}
