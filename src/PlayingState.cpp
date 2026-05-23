#include "PlayingState.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <stack>

#include "Cell.hpp"
#include "LostState.hpp"
#include "RandomMineGenerator.hpp"
#include "WonState.hpp"

PlayingState::PlayingState(State::Context const &ctx, PlaySettings settings)
    : State(ctx), cols{settings.getCols()}, rows{settings.getRows()},
      grid{cols, rows, ctx},
      totalCellCount{static_cast<unsigned int>(cols * rows)},
      mineCellCount{static_cast<unsigned int>(
          static_cast<float>(totalCellCount) * settings.getMineDensity())},
      safeCellCount{totalCellCount - mineCellCount}, revealedCellCount(0),
      header{ctx}
{
    float windowWidth = static_cast<float>(cols) * CellGrid::CELL_SIZE;
    float windowHeight = static_cast<float>(rows) * CellGrid::CELL_SIZE;
    sf::Vector2f windowSize = {windowWidth, windowHeight};
    sf::View view{sf::FloatRect{{0.0f, 0.0f}, {windowWidth, windowHeight}}};

    ctx.getWindow().get().setSize(sf::Vector2u{windowSize});
    ctx.getWindow().get().setView(view);
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
        sf::Vector2f gridPos =
            ctx.getWindow().get().mapPixelToCoords(mouse->position);

        auto col = static_cast<int>(gridPos.x / CellGrid::CELL_SIZE);
        auto row = static_cast<int>(gridPos.y / CellGrid::CELL_SIZE);

        Cell *cell = grid.getCell(col, row);
        if (!cell)
        {
            return;
        }

        if (mouse->button == sf::Mouse::Button::Left)
        {
            onCellLeftClick(col, row);
        }
        else if (mouse->button == sf::Mouse::Button::Right)
        {
            onCellRightClick(col, row);
        }
    }
}

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(header, states);

    auto gridStates = states;
    gridStates.transform.translate({0, PlayingHeader::HEIGHT});
    target.draw(grid, gridStates);
}

std::optional<State::Transition> PlayingState::getTransition()
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    // TODO: make sure that a transition occurs right after getTransition
    // returns != nulloptr
    if (playingStatus == Status::Lost)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state =
            std::make_unique<LostState>(ctx, std::move(grid), detonatedPos);
        return transition;
    }

    if (playingStatus == Status::Won)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<WonState>(ctx, std::move(grid));
        return transition;
    }

    return std::nullopt;
}

void PlayingState::print(std::ostream &os) const
{
    os << "PlayingState[grid=" << grid << "]";
}

void PlayingState::onCellLeftClick(int col, int row)
{
    Cell *cell = grid.getCell(col, row);

    if (revealedCellCount == 0)
    {
        onFirstReveal(col, row);
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

    if (revealedCellCount == safeCellCount)
    {
        playingStatus = Status::Won;
    }
}

void PlayingState::onCellRightClick(int col, int row)
{
    Cell *cell = grid.getCell(col, row);

    if (cell->isRevealed())
    {
        return;
    }

    cell->setFlagged(!cell->isFlagged());
}

void PlayingState::onFirstReveal(int col, int row)
{
    RandomMineGenerator generator{grid, mineCellCount};
    generator.generateSafeStart(col, row);
    floodReveal(col, row);
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
            revealedCellCount++;
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
        revealedCellCount++;

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
