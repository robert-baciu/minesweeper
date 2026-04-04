#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Cell.hpp"

class Grid
{
  public:
    Grid(unsigned int cols, unsigned int rows);
    void placeRandMines(sf::Vector2u startCell, unsigned int mineCount);
    inline Cell &getCell(unsigned int col, unsigned int row);
    inline Cell const &getCell(unsigned int col, unsigned int row) const;
    inline Cell &getCell(sf::Vector2u pos);
    inline Cell const &getCell(sf::Vector2u pos) const;
    std::vector<Cell> const &getCells() const;

    friend std::ostream &operator<<(std::ostream &os, Grid const &grid);

  private:
    unsigned int const cols;
    unsigned int const rows;
    std::vector<Cell> cells;
};

Grid::Grid(unsigned int cols, unsigned int rows) : cols(cols), rows(rows)
{
    cells.resize(cols * rows);
}

void Grid::placeRandMines(sf::Vector2u startCell, unsigned int mineCount)
{
    unsigned int placed = 0;
    while (placed < mineCount)
    {
        // TODO: replace with C++11 random library
        unsigned int col = rand() % cols;
        unsigned int row = rand() % rows;

        if (startCell.x == col && startCell.y == row)
        {
            continue;
        }

        Cell &cell = getCell(col, row);
        if (cell.getType() == Cell::Type::Empty)
        {
            cell.setType(Cell::Type::Mine);
            placed++;
        }
    }
}

inline Cell &Grid::getCell(unsigned int const col, unsigned int const row)
{
    return cells[col + row * cols];
}

inline Cell const &Grid::getCell(unsigned int const col,
                                 unsigned int const row) const
{
    return cells[col + row * cols];
}

inline Cell const &Grid::getCell(sf::Vector2u pos) const
{
    return cells[pos.x + pos.y * cols];
}

inline Cell &Grid::getCell(sf::Vector2u pos)
{
    return cells[pos.x + pos.y * cols];
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
