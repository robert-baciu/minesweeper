#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Cell.hpp"

class CellGrid
{
  public:
    CellGrid(int cols, int rows);

    [[nodiscard]] int getCols() const;
    [[nodiscard]] int getRows() const;

    [[nodiscard]] Cell *getCell(int col, int row);
    [[nodiscard]] Cell const *getCell(int col, int row) const;
    [[nodiscard]] std::vector<Cell> const &getCells() const;

    // void forEachNeighbor(int col, int row,
    //                      std::function<void(Cell *)> const &callback);
    std::vector<Cell *> getNeighbors(int col, int row);

    friend std::ostream &operator<<(std::ostream &os, CellGrid const &grid);

  private:
    int const cols;
    int const rows;
    std::vector<Cell> cells;

    [[nodiscard]] bool isInBounds(int col, int row) const;
};
