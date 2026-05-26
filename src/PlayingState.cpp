#include "PlayingState.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <stack>

#include "Cell.hpp"
#include "LostState.hpp"
#include "RandomMineGenerator.hpp"
#include "WonState.hpp"

PlayingState::PlayingState(GameStateCtxPtr gameCtx_)
    : GameState(std::move(gameCtx_)),
      revealedCount(0),
      flagCount(0),
      lost(false),
      won(false),
      detonatedPos(sf::Vector2f(0, 0))
{
    gameCtx->getWindow().get().setSize(sf::Vector2u(gameCtx->getWindowSize()));
    gameCtx->getWindow().get().setView(
        sf::View(sf::FloatRect(sf::Vector2f(0, 0), gameCtx->getWindowSize())));

    gameCtx->getHeader().setHeaderSize(gameCtx->getHeaderSize());
    gameCtx->getHeaderView().setSize(gameCtx->getHeaderSize());
    gameCtx->getHeaderView().setCenter(gameCtx->getHeaderSize() / 2.0f);
    gameCtx->getHeaderView().setViewport(sf::FloatRect(
        sf::Vector2f(gameCtx->getHeaderPos().x / gameCtx->getWindowSize().x,
                     gameCtx->getHeaderPos().y / gameCtx->getWindowSize().y),
        sf::Vector2f(gameCtx->getHeaderSize().x / gameCtx->getWindowSize().x,
                     gameCtx->getHeaderSize().y / gameCtx->getWindowSize().y)));

    gameCtx->getGridView().setSize(gameCtx->getGridSize());
    gameCtx->getGridView().setCenter(gameCtx->getGridSize() / 2.0f);
    gameCtx->getGridView().setViewport(sf::FloatRect(
        sf::Vector2f(gameCtx->getGridPos().x / gameCtx->getWindowSize().x,
                     gameCtx->getGridPos().y / gameCtx->getWindowSize().y),
        sf::Vector2f(gameCtx->getGridSize().x / gameCtx->getWindowSize().x,
                     gameCtx->getGridSize().y / gameCtx->getWindowSize().y)));
}

void PlayingState::handleEvent(std::optional<sf::Event> const &event)
{
    if (lost || won)
    {
        return;
    }

    if (event->is<sf::Event::MouseButtonPressed>())
    {
        auto const *mouse = event->getIf<sf::Event::MouseButtonPressed>();

        sf::Vector2f headerMousePos =
            gameCtx->getWindow().get().mapPixelToCoords(
                mouse->position, gameCtx->getHeaderView());

        if (mouse->button == sf::Mouse::Button::Left &&
            gameCtx->getHeader().getSmiley().getGlobalBounds().contains(
                headerMousePos))
        {
            restart = true;
            return;
        }

        sf::Vector2f gridMousePos = gameCtx->getWindow().get().mapPixelToCoords(
            mouse->position, gameCtx->getGridView());

        sf::FloatRect gridBounds(sf::Vector2f(0, 0), gameCtx->getGridSize());

        if (gridBounds.contains(gridMousePos))
        {
            auto col =
                static_cast<int>(gridMousePos.x / PlayingGrid::CELL_SIZE);
            auto row =
                static_cast<int>(gridMousePos.y / PlayingGrid::CELL_SIZE);

            Cell *cell = gameCtx->getGrid().getCell(col, row);
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

// TODO: PausedState on Esc - MENU & EXIT buttons

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.setView(gameCtx->getHeaderView());
    target.draw(gameCtx->getHeader(), states);

    target.setView(gameCtx->getGridView());
    target.draw(gameCtx->getGrid(), states);
}

void PlayingState::update([[maybe_unused]] double dt)
{
    gameCtx->getHeader().setRemainingMines(
        static_cast<int>(gameCtx->getMineCount()) -
        static_cast<int>(flagCount));
    gameCtx->getHeader().update();
}

std::optional<State::Transition> PlayingState::getTransition()
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (restart)
    {
        auto newGameCtx = std::make_shared<GameStateCtx>(
            ctx, gameCtx->getDifficulty(), gameCtx->getParams());

        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state =
            std::make_unique<PlayingState>(std::move(newGameCtx));
        return transition;
    }

    if (lost)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state =
            std::make_unique<LostState>(std::move(gameCtx), detonatedPos);
        return transition;
    }

    if (won)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<WonState>(std::move(gameCtx));
        return transition;
    }

    return std::nullopt;
}

void PlayingState::print(std::ostream &os) const
{
    os << "PlayingState";
}

void PlayingState::gridLeftClick(int col, int row)
{
    Cell *cell = gameCtx->getGrid().getCell(col, row);

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
            lost = true;
            detonatedPos = {col, row};
            return;
        }
        else
        {
            floodReveal(col, row);
        }
    }

    if (revealedCount == gameCtx->getCellCount() - gameCtx->getMineCount())
    {
        won = true;
    }
}

void PlayingState::gridRightClick(int col, int row)
{
    Cell *cell = gameCtx->getGrid().getCell(col, row);

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
    RandomMineGenerator generator{gameCtx->getGrid(), gameCtx->getMineCount()};
    generator.generateSafeStart(col, row);
    floodReveal(col, row);

    gameCtx->getHeader().startClock();
}

void PlayingState::chordingReveal(int col, int row)
{
    Cell *cell = gameCtx->getGrid().getCell(col, row);

    unsigned int adjacentMines = cell->getAdjacentMines();
    unsigned int flags = 0;

    // clang-format off
    gameCtx->getGrid().neighbors(col, row, [this, &flags](int neighborCol, int neighborRow)
    {
        Cell *neighbor = gameCtx->getGrid().getCell(neighborCol, neighborRow);
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
    gameCtx->getGrid().neighbors(col, row, [this](int neighborCol, int neighborRow) {
        Cell *neighbor = gameCtx->getGrid().getCell(neighborCol, neighborRow);
        if (!neighbor || neighbor->isRevealed() || neighbor->isFlagged())
        {
            return;
        }

        if (neighbor->isMine())
        {
            lost = true;
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

        Cell *cell = gameCtx->getGrid().getCell(cellPos.x, cellPos.y);

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
