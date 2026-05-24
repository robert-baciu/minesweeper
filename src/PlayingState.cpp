#include "PlayingState.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <stack>

#include "Cell.hpp"
#include "LostState.hpp"
#include "RandomMineGenerator.hpp"
#include "WonState.hpp"

PlayingState::PlayingState(State::Context const &ctx,
                           DifficultySettings settings)
    : State(ctx),
      cols(settings.getCols()),
      rows(settings.getRows()),

      gridSize(static_cast<float>(cols) * CellGrid::CELL_SIZE,
               static_cast<float>(rows) * CellGrid::CELL_SIZE),

      winSizeFloat(PADDING.x + gridSize.x + PADDING.x,
                   PADDING.y + HEADER_H + PADDING.y + gridSize.y + PADDING.y),
      winSize(sf::Vector2u(winSizeFloat)),

      header(ctx),
      grid(cols, rows, ctx),

      cellCount(cols * rows),
      mineCount(static_cast<unsigned int>(static_cast<float>(cellCount) *
                                          settings.getMineDensity())),
      revealedCount(0),
      flagCount(0),
      playingStatus(Status::Ongoing)
{
    ctx.getWindow().get().setSize(winSize);
    ctx.getWindow().get().setView(
        sf::View(sf::FloatRect(sf::Vector2f(), winSizeFloat)));

    auto headerPos = sf::Vector2f(PADDING.x, PADDING.y);
    auto headerSize = sf::Vector2f(winSizeFloat.x - 2 * PADDING.x, HEADER_H);

    auto gridPos =
        sf::Vector2f(PADDING.x, headerPos.y + headerSize.y + PADDING.y);

    header.setHeaderSize(headerSize);
    headerView.setSize(headerSize);
    headerView.setCenter(headerSize / 2.0f);
    headerView.setViewport(
        sf::FloatRect(sf::Vector2f(headerPos.x / winSizeFloat.x,
                                   headerPos.y / winSizeFloat.y),
                      sf::Vector2f(headerSize.x / winSizeFloat.x,
                                   headerSize.y / winSizeFloat.y)));

    gridView.setSize(gridSize);
    gridView.setCenter(gridSize / 2.0f);
    gridView.setViewport(sf::FloatRect(
        sf::Vector2f(gridPos.x / winSizeFloat.x, gridPos.y / winSizeFloat.y),
        sf::Vector2f(gridSize.x / winSizeFloat.x,
                     gridSize.y / winSizeFloat.y)));
}

void PlayingState::handleEvent(std::optional<sf::Event> const &event)
{
    if (playingStatus != Status::Ongoing)
    {
        return;
    }

    if (event->is<sf::Event::MouseButtonPressed>())
    {
        auto const *mouse = event->getIf<sf::Event::MouseButtonPressed>();

        sf::Vector2f headerPos =
            ctx.getWindow().get().mapPixelToCoords(mouse->position, headerView);

        if (mouse->button == sf::Mouse::Button::Left &&
            header.getSmiley().getGlobalBounds().contains(headerPos))
        {
            std::cout << "RESET GAME\n";
            return;
        }

        sf::Vector2f gridPos =
            ctx.getWindow().get().mapPixelToCoords(mouse->position, gridView);

        sf::FloatRect gridBounds{sf::Vector2f(), gridSize};

        if (gridBounds.contains(gridPos))
        {
            auto col = static_cast<int>(gridPos.x / CellGrid::CELL_SIZE);
            auto row = static_cast<int>(gridPos.y / CellGrid::CELL_SIZE);

            std::cout << gridPos.x << ' ' << gridPos.y << '\n';

            Cell *cell = grid.getCell(col, row);
            if (!cell)
            {
                return;
            }

            if (mouse->button == sf::Mouse::Button::Left)
            {
                gridLeftClick(col, row);
            }
            else if (mouse->button == sf::Mouse::Button::Right)
            {
                gridRightClick(col, row);
            }
        }
    }
}

// TODO: pause on Esc and exit/main menu with two separate buttons

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.setView(headerView);
    target.draw(header, states);

    target.setView(gridView);
    target.draw(grid, states);
}

void PlayingState::update([[maybe_unused]] double dt)
{
    header.setRemainingMines(static_cast<int>(mineCount) -
                             static_cast<int>(flagCount));
    header.update();
}

std::optional<State::Transition> PlayingState::getTransition()
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (playingStatus == Status::Lost)
    {
        WindowLayout layout = {.header = header,
                               .headerView = headerView,
                               .grid = grid,
                               .gridView = gridView};

        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state =
            std::make_unique<LostState>(ctx, layout, detonatedPos);
        return transition;
    }

    if (playingStatus == Status::Won)
    {
        WindowLayout layout = {.header = header,
                               .headerView = headerView,
                               .grid = grid,
                               .gridView = gridView};

        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<WonState>(ctx, layout);
        return transition;
    }

    return std::nullopt;
}

void PlayingState::print(std::ostream &os) const
{
    os << "PlayingState[grid=" << grid << "]";
}

void PlayingState::gridLeftClick(int col, int row)
{
    Cell *cell = grid.getCell(col, row);

    if (revealedCount == 0)
    {
        firstReveal(col, row);
    }
    else if (cell->isRevealed())
    {
        chordingReveal(col, row);
    }
    else
    {
        if (cell->isMine())
        {
            playingStatus = Status::Lost;
            detonatedPos = {col, row};
            return;
        }
        else
        {
            floodReveal(col, row);
        }
    }

    if (revealedCount == cellCount - mineCount)
    {
        playingStatus = Status::Won;
    }
}

void PlayingState::gridRightClick(int col, int row)
{
    Cell *cell = grid.getCell(col, row);

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

void PlayingState::firstReveal(int col, int row)
{
    RandomMineGenerator generator{grid, mineCount};
    generator.generateSafeStart(col, row);
    floodReveal(col, row);

    header.startClock();
}

void PlayingState::chordingReveal(int col, int row)
{
    Cell *cell = grid.getCell(col, row);

    unsigned int adjacentMines = cell->getAdjacentMines();
    unsigned int flags = 0;

    // clang-format off
    grid.neighbors(col, row, [this, &flags](int neighborCol, int neighborRow)
    {
        Cell *neighbor = grid.getCell(neighborCol, neighborRow);
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
    grid.neighbors(col, row, [this](int neighborCol, int neighborRow) {
        Cell *neighbor = grid.getCell(neighborCol, neighborRow);
        if (!neighbor || neighbor->isRevealed() || neighbor->isFlagged())
        {
            return;
        }

        if (neighbor->isMine())
        {
            playingStatus = Status::Lost;
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

void PlayingState::floodReveal(int col, int row)
{
    std::stack<sf::Vector2i> stack;
    stack.emplace(col, row);

    while (!stack.empty())
    {
        auto cellPos = stack.top();
        stack.pop();

        Cell *cell = grid.getCell(cellPos.x, cellPos.y);

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
