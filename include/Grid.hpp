#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Cell.hpp"

class Grid
{
  public:
    Grid(int cols, int rows);
    void placeRandMines(int col, int row, unsigned int mineCount);
    std::vector<Cell> const &getCells() const;

    inline Cell *getCell(int col, int row)
    {
        if (col < 0 || col >= static_cast<int>(cols) || row < 0 ||
            row >= static_cast<int>(rows))
        {
            return nullptr;
        }

        return &cells[static_cast<size_t>(col) +
                      static_cast<size_t>(row) * cols];
    }

    inline Cell const *getCell(int col, int row) const
    {
        if (col < 0 || col >= static_cast<int>(cols) || row < 0 ||
            row >= static_cast<int>(rows))
        {
            return nullptr;
        }

        return &cells[static_cast<size_t>(col) +
                      static_cast<size_t>(row) * cols];
    }

    friend std::ostream &operator<<(std::ostream &os, Grid const &grid);

  private:
    int const cols;
    int const rows;
    std::vector<Cell> cells;

    std::vector<Cell *> getCellNeighbors(int col, int row);
};
