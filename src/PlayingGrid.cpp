#include "PlayingGrid.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "RandomMineGenerator.hpp"

PlayingGrid::PlayingGrid(StateCtx const &ctx, DifficultyParams params,
                         sf::Vector2f position)
    : ctx(ctx),
      cols(params.getCols()),
      rows(params.getRows()),
      size(PlayingGrid::CELL_SIZE *
           sf::Vector2f(static_cast<float>(cols), static_cast<float>(rows))),
      position(position),

      cells(cols * rows),

      gridState(GridState::Ongoing),

      cellText(ctx.getAssets().getMainFont()),

      flagSprite(ctx.getAssets().getTexture("flag")),
      mineSprite(ctx.getAssets().getTexture("mine")),

      mineCount(static_cast<unsigned int>(static_cast<float>(cells.size()) *
                                          params.getMineDensity())),
      revealedCount(0),
      flagCount(0),

      started(false)
{
    cellText.setCharacterSize(CELL_DIGIT_SIZE);
}

void PlayingGrid::handleEvent(std::optional<sf::Event> const &event)
{
    if (gridState != GridState::Ongoing)
    {
        return;
    }

    if (auto const *mouse = event->getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f globalPos =
            ctx.getWindow().get().mapPixelToCoords(mouse->position);
        sf::Vector2f localPos = globalPos - position;

        sf::FloatRect bounds(sf::Vector2f(0, 0), size);

        if (!bounds.contains(localPos))
        {
            return;
        }

        auto col = static_cast<int>(localPos.x / PlayingGrid::CELL_SIZE);
        auto row = static_cast<int>(localPos.y / PlayingGrid::CELL_SIZE);

        Cell *cell = getCell(col, row);
        if (!cell)
        {
            return;
        }

        if (mouse->button == sf::Mouse::Button::Left)
        {
            leftClick(col, row);
        }
        else if (mouse->button == sf::Mouse::Button::Right)
        {
            rightClick(col, row);
        }
    }
}

void PlayingGrid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform.translate(position);

    // clang-format off
    all([&](int col, int row) {
        Cell const *cell = getCell(col, row);

        sf::Vector2f cellPos = CELL_SIZE * sf::Vector2f(sf::Vector2i(col, row));

        sf::RenderStates cellStates = states;
        cellStates.transform.translate(cellPos);

        sf::RectangleShape cellRect = getCellRect();
        cellRect.setFillColor(cell->isRevealed() ? CELL_COLOR_REVEALED
                                                  : CELL_COLOR_UNREVEALED);
        target.draw(cellRect, cellStates);

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
            cellText.setPosition(sf::Vector2f(CELL_SIZE, CELL_SIZE) / 2.0f);
            cellText.setFillColor(CELL_DIGIT_COLORS[adjacentMines]);

            target.draw(cellText, cellStates);
        }
    });
    // clang-format on
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

sf::Vector2f PlayingGrid::getSize() const
{
    return size;
}

sf::Vector2f PlayingGrid::getPosition() const
{
    return position;
}

PlayingGrid::GridState PlayingGrid::getState() const
{
    return gridState;
}

sf::Vector2i PlayingGrid::getDetonatedPos() const
{
    return detonatedPos;
}

sf::Sprite PlayingGrid::getFlagSprite() const
{
    return flagSprite;
}

sf::Sprite PlayingGrid::getMineSprite() const
{
    return mineSprite;
}

sf::RectangleShape PlayingGrid::getCellRect()
{
    return sf::RectangleShape(
        sf::Vector2f(CELL_SIZE - CELL_PADDING, CELL_SIZE - CELL_PADDING));
}

unsigned int PlayingGrid::getMineCount() const
{
    return mineCount;
}

unsigned int PlayingGrid::getFlagCount() const
{
    return flagCount;
}

bool PlayingGrid::isStarted() const
{
    return started;
}

std::ostream &operator<<(std::ostream &os, PlayingGrid const &grid)
{
    os << "Grid[cols=" << grid.cols << ", rows=" << grid.rows << "]";
    return os;
}

void PlayingGrid::leftClick(int col, int row)
{
    Cell *cell = getCell(col, row);

    if (revealedCount == 0)
    {
        firstReveal(col, row);
    }
    else if (cell->isRevealed())
    {
        chordReveal(col, row);
    }
    else
    {
        if (cell->isMine())
        {
            gridState = GridState::Lost;
            detonatedPos = {col, row};
            return;
        }
        else
        {
            floodReveal(col, row);
        }
    }

    if (revealedCount == cells.size() - mineCount)
    {
        gridState = GridState::Won;
    }
}

void PlayingGrid::rightClick(int col, int row)
{
    Cell *cell = getCell(col, row);

    if (cell->isRevealed())
    {
        return;
    }

    if (cell->isFlagged())
    {
        cell->setFlagged(false);
        flagCount--;
    }
    else
    {
        cell->setFlagged(true);
        flagCount++;
    }
}

void PlayingGrid::firstReveal(int col, int row)
{
    RandomMineGenerator generator(*this, mineCount);
    generator.generateSafeStart(col, row);
    floodReveal(col, row);

    started = true;
}

// FIXME: Flood reveal removes placed flags on very high mine densities, without
// revealing those cells
void PlayingGrid::floodReveal(int col, int row)
{
    std::stack<sf::Vector2i> stack;
    stack.emplace(col, row);

    while (!stack.empty())
    {
        auto cellPos = stack.top();
        stack.pop();

        Cell *cell = getCell(cellPos.x, cellPos.y);

        if (!cell || cell->isRevealed())
        {
            continue;
        }

        cell->setRevealed();
        revealedCount++;

        if (cell->getAdjacentMines() > 0)
        {
            continue;
        }

        for (int rowOff = -1; rowOff <= 1; rowOff++)
        {
            for (int colOff = -1; colOff <= 1; colOff++)
            {
                if (colOff == 0 && rowOff == 0)
                {
                    continue;
                }

                stack.emplace(cellPos.x + colOff, cellPos.y + rowOff);
            }
        }
    }
}

void PlayingGrid::chordReveal(int col, int row)
{
    Cell *cell = getCell(col, row);

    unsigned int adjacentMines = cell->getAdjacentMines();
    unsigned int flags = 0;

    // clang-format off
    neighbors(col, row, [this, &flags](int neighborCol, int neighborRow)
    {
        Cell *neighbor = getCell(neighborCol, neighborRow);
        if (neighbor->isFlagged())
        {
            flags++;
        }
    });
    // clang-format on

    if (flags != adjacentMines)
    {
        return;
    }

    // clang-format off
    neighbors(col, row, [this](int neighborCol, int neighborRow) {
        Cell *neighbor = getCell(neighborCol, neighborRow);
        if (!neighbor || neighbor->isRevealed() || neighbor->isFlagged())
        {
            return;
        }

        if (neighbor->isMine())
        {
            gridState = GridState::Lost;
            detonatedPos = {neighborCol, neighborRow};
            return;
        }

        if (neighbor->getAdjacentMines() == 0)
        {
            floodReveal(neighborCol, neighborRow);
        }
        else
        {
            neighbor->setRevealed();
            revealedCount++;
        }
    });
    // clang-format on
}

bool PlayingGrid::isInBounds(int col, int row) const
{
    return col >= 0 && col < cols && row >= 0 && row < rows;
}
