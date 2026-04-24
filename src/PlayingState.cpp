#include "PlayingState.hpp"

#include <SFML/Window/Mouse.hpp>
#include <stack>
#include <utility>

#include "Cell.hpp"
#include "LostState.hpp"
#include "RandomMineGenerator.hpp"
#include "WonState.hpp"

PlayingState::PlayingState(StateContext const &ctx, int cols, int rows)
    : State(ctx), cols(cols), rows(rows), grid(cols, rows),
      mineGenerator(getMineCount()),
      cellShape{{CELL_SIZE - CELL_PADDING, CELL_SIZE - CELL_PADDING}},
      cellText(ctx.assets.getMainFont())
{
    totalSafeCells = cols * rows - getMineCount();

    float windowWidth = static_cast<float>(cols) * CELL_SIZE;
    float windowHeight = static_cast<float>(rows) * CELL_SIZE;
    sf::Vector2f windowSize = {windowWidth, windowHeight};
    sf::View view{sf::FloatRect{{0.0f, 0.0f}, {windowWidth, windowHeight}}};

    ctx.window.get().setSize(sf::Vector2u{windowSize});
    ctx.window.get().setView(view);

    cellText.setCharacterSize(CELL_TEXT_CHAR_SIZE);
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
            ctx.window.get().mapPixelToCoords(mouse->position);

        auto col = static_cast<int>(gridPos.x / CELL_SIZE);
        auto row = static_cast<int>(gridPos.y / CELL_SIZE);

        Cell *cell = grid.getCell(col, row);
        if (!cell)
        {
            return;
        }

        if (mouse->button == sf::Mouse::Button::Left)
        {
            revealCell(cell, col, row);
        }
        else if (mouse->button == sf::Mouse::Button::Right)
        {
            toggleFlag(cell);
        }
    }
}

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Transform initialTransform = states.transform;

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            Cell const *cell = grid.getCell(col, row);

            states.transform.translate(
                sf::Vector2f{static_cast<float>(col), static_cast<float>(row)} *
                CELL_SIZE);

            cellShape.setFillColor(getCellColor(cell->getState()));

            target.draw(cellShape, states);

            if (cell->getState() == Cell::State::Revealed &&
                cell->getType() == Cell::Type::Empty &&
                cell->getMineCount() > 0)
            {
                auto mineCount = cell->getMineCount();
                cellText.setString(std::to_string(mineCount));
                cellText.setOrigin(cellText.getLocalBounds().getCenter());
                cellText.setPosition(
                    sf::Vector2f{CELL_SIZE / 2.0f, CELL_SIZE / 2.0f});
                cellText.setFillColor(getCellTextColor(mineCount));

                target.draw(cellText, states);
            }

            states.transform = initialTransform;
        }
    }
}

std::optional<State::Transition> PlayingState::getTransition() const
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (playingStatus == Status::Lost)
    {
        State::Transition transition;
        transition.action = State::Action::Push;
        transition.state = std::make_unique<LostState>(ctx);
        return transition;
    }

    if (playingStatus == Status::Won)
    {
        State::Transition transition;
        transition.action = State::Action::Push;
        transition.state = std::make_unique<WonState>(ctx);
        return transition;
    }

    return std::nullopt;
}

void PlayingState::print(std::ostream &os) const
{
    os << "PlayingState[grid=" << grid << "]";
}

void PlayingState::revealCell(Cell *cell, int col, int row)
{
    if (revealedCellCount == 0)
    {
        mineGenerator.generate(grid, col, row);
        revealFlood(col, row);
        if (revealedCellCount == totalSafeCells)
        {
            playingStatus = Status::Won;
        }

        return;
    }

    if (cell->getState() != Cell::State::Revealed)
    {
        if (cell->getType() == Cell::Type::Empty)
        {
            revealFlood(col, row);
            if (revealedCellCount == totalSafeCells)
            {
                playingStatus = Status::Won;
            }
        }
        else if (cell->getType() == Cell::Type::Mine)
        {
            playingStatus = Status::Lost;
        }
    }
}

void PlayingState::toggleFlag(Cell *cell)
{
    if (cell->getState() == Cell::State::Unrevealed)
    {
        cell->setState(Cell::State::Flagged);
    }
    else if (cell->getState() == Cell::State::Flagged)
    {
        cell->setState(Cell::State::Unrevealed);
    }
}

void PlayingState::revealFlood(int startCol, int startRow)
{
    std::stack<sf::Vector2i> stack;

    stack.emplace(startCol, startRow);

    while (!stack.empty())
    {
        std::cout << stack.size() << std::endl;

        auto cellPos = stack.top();
        stack.pop();

        Cell *cell = grid.getCell(cellPos.x, cellPos.y);

        if (!cell || cell->getState() == Cell::State::Revealed)
        {
            continue;
        }

        cell->setState(Cell::State::Revealed);
        revealedCellCount++;

        // getMineCount can be called because the current cell is not a Mine
        // The flood fill will not propagate to a neighbor that could be a Mine
        if (cell->getMineCount() > 0)
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

unsigned int PlayingState::getMineCount() const
{
    return (cols * rows) / 8;
}

sf::Color PlayingState::getCellColor(Cell::State state)
{
    switch (state)
    {
    case Cell::State::Unrevealed:
        return {127, 127, 127};
    case Cell::State::Revealed:
        return {190, 190, 190};
    case Cell::State::Flagged:
        return {190, 190, 0};
    default:
        std::unreachable();
    }
}

sf::Color PlayingState::getCellTextColor(unsigned int mineCount)
{
    switch (mineCount)
    {
    case 1:
        return sf::Color{0, 0, 127};
    case 2:
        return sf::Color{0, 127, 0};
    case 3:
        return sf::Color{190, 0, 0};
    case 4:
        return sf::Color{0, 0, 63};
    case 5:
        return sf::Color{127, 0, 0};
    case 6:
        return sf::Color{0, 127, 127};
    case 7:
        return sf::Color{127, 0, 127};
    case 8:
        return sf::Color{63, 63, 63};
    default:
        std::unreachable();
    }
}
