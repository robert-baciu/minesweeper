#include "PlayingGrid.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

PlayingGrid::PlayingGrid(StateCtxPtr const &ctx, int cols, int rows)
    : cols(cols),
      rows(rows),
      cells(cols * rows),
      cellShape(
          sf::Vector2f(CELL_SIZE - CELL_PADDING, CELL_SIZE - CELL_PADDING)),
      cellText(ctx->getAssets().getMainFont()),
      flagSprite(ctx->getAssets().getTexture("flag")),
      mineSprite(ctx->getAssets().getTexture("mine"))
{
    cellText.setCharacterSize(CELL_DIGIT_SIZE);
}

int PlayingGrid::getCols() const
{
    return cols;
}

int PlayingGrid::getRows() const
{
    return rows;
}

Cell *PlayingGrid::getCell(int col, int row)
{
    return isInBounds(col, row) ? &cells[col + row * cols] : nullptr;
}

Cell const *PlayingGrid::getCell(int col, int row) const
{
    return isInBounds(col, row) ? &cells[col + row * cols] : nullptr;
}

std::vector<Cell> const &PlayingGrid::getCells() const
{
    return cells;
}

void PlayingGrid::all(
    std::function<void(int col, int row)> const &callback) const
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            callback(col, row);
        }
    }
}

void PlayingGrid::neighbors(
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

void PlayingGrid::draw(sf::RenderTarget &target, sf::RenderStates states) const
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

sf::Sprite PlayingGrid::getFlagSprite() const
{
    return flagSprite;
}

sf::Sprite PlayingGrid::getMineSprite() const
{
    return mineSprite;
}

std::ostream &operator<<(std::ostream &os, PlayingGrid const &grid)
{
    os << "Grid[cols=" << grid.cols << ", rows=" << grid.rows << "]";
    return os;
}

bool PlayingGrid::isInBounds(int col, int row) const
{
    return col >= 0 && col < cols && row >= 0 && row < rows;
}
