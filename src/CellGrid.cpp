#include "CellGrid.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

CellGrid::CellGrid(int cols, int rows, State::Context const &ctx)
    : cols(cols), rows(rows), cells(cols * rows),
      cellShape{{CELL_SIZE - CELL_PADDING, CELL_SIZE - CELL_PADDING}},
      cellText{ctx.getAssets().getMainFont(), "", CELL_DIGIT_SIZE},
      flagSprite{ctx.getAssets().getTexture("flag")}
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

void CellGrid::all(std::function<void(int col, int row)> const &callback) const
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            callback(col, row);
        }
    }
}

void CellGrid::neighbors(
    int col, int row,
    std::function<void(int neighborCol, int neighborRow)> const &callback) const
{
    for (int rowOff = -1; rowOff <= 1; rowOff++)
    {
        for (int colOff = -1; colOff <= 1; colOff++)
        {
            if (colOff == 0 && rowOff == 0)
            {
                continue;
            }

            int neighborCol = col + colOff;
            int neighborRow = row + rowOff;
            if (!isInBounds(neighborCol, neighborRow))
            {
                continue;
            }

            callback(neighborCol, neighborRow);
        }
    }
}

void CellGrid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // clang-format off
    all([&](int col, int row) {
        Cell const *cell = getCell(col, row);
        auto cellPos = sf::Vector2f{sf::Vector2i{col, row}} * CELL_SIZE;
        auto cellCenterOff =
            sf::Vector2f{CELL_SIZE / 2.0f, CELL_SIZE / 2.0f};

        auto cellStates = states;
        cellStates.transform.translate(cellPos);

        cellShape.setFillColor(cell->isRevealed() ? CELL_COLOR_REVEALED
                                                  : CELL_COLOR_UNREVEALED);
        target.draw(cellShape, cellStates);

        if (cell->isFlagged())
        {
            target.draw(flagSprite, cellStates);
        }

        if (cell->isRevealed() && !cell->isMine() &&
            cell->getAdjacentMines() > 0)
        {
            auto adjacentMines = cell->getAdjacentMines();
            cellText.setString(std::to_string(adjacentMines));
            cellText.setOrigin(cellText.getLocalBounds().getCenter());
            cellText.setPosition(cellCenterOff);
            cellText.setFillColor(CELL_DIGIT_COLORS[adjacentMines]);

            target.draw(cellText, cellStates);
        }
    });
    // clang-format on
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
