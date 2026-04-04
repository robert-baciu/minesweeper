#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Cell.hpp"

class Grid
{
  public:
    Grid(int cols, int rows);
    void placeRandMines(sf::Vector2i startCell, unsigned int mineCount);
    inline Cell *getCell(int col, int row);
    inline Cell const *getCell(int col, int row) const;
    std::vector<Cell> const &getCells() const;

    friend std::ostream &operator<<(std::ostream &os, Grid const &grid);

  private:
    int const cols;
    int const rows;
    std::vector<Cell> cells;

    std::vector<Cell *> getCellNeighbors(int col, int row);
};

Grid::Grid(int cols, int rows) : cols(cols), rows(rows)
{
    cells.resize(cols * rows);
}

void Grid::placeRandMines(sf::Vector2i startCell, unsigned int mineCount)
{
    unsigned int placedCount = 0;
    while (placedCount < mineCount)
    {
        // TODO: replace with C++11 random library
        int col = rand() % cols;
        int row = rand() % rows;

        // TODO: avoid mines *around* the first revealed cell
        if (startCell.x == col && startCell.y == row)
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

inline Cell *Grid::getCell(int col, int row)
{
    if (col < 0 || col >= static_cast<int>(cols) || row < 0 ||
        row >= static_cast<int>(rows))
    {
        return nullptr;
    }

    return &cells[static_cast<size_t>(col) + static_cast<size_t>(row) * cols];
}

inline Cell const *Grid::getCell(int col, int row) const
{
    if (col < 0 || col >= static_cast<int>(cols) || row < 0 ||
        row >= static_cast<int>(rows))
    {
        return nullptr;
    }

    return &cells[static_cast<size_t>(col) + static_cast<size_t>(row) * cols];
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
