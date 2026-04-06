#include "PlayingState.hpp"

#include <SFML/Window/Mouse.hpp>
#include <utility>

#include "Cell.hpp"
#include "RandomMineGenerator.hpp"

PlayingState::PlayingState(StateContext const &ctx, int cols, int rows)
    : GameState(ctx), cols(cols), rows(rows), grid(cols, rows),
      mineGenerator(getMineCount(), START_SAFE_DISTANCE),
      cellShape{{CELL_SIZE - CELL_PADDING, CELL_SIZE - CELL_PADDING}},
      cellText(ctx.assets.getMainFont())
{
    float windowWidth = static_cast<float>(cols) * CELL_SIZE;
    float windowHeight = static_cast<float>(rows) * CELL_SIZE;
    sf::Vector2f windowSize = {windowWidth, windowHeight};
    sf::View view{sf::FloatRect{{0.0f, 0.0f}, {windowWidth, windowHeight}}};

    ctx.window.get().setSize(sf::Vector2u{windowSize});
    ctx.window.get().setView(view);

    cellText.setCharacterSize(CELL_TEXT_CHAR_SIZE);
}

void PlayingState::update([[maybe_unused]] double dt)
{
}

void PlayingState::handleEvent(std::optional<sf::Event> const &event)
{
    if (event->is<sf::Event::MouseButtonPressed>())
    {
        auto const *mouse = event->getIf<sf::Event::MouseButtonPressed>();
        sf::Vector2f gridPos =
            ctx.window.get().mapPixelToCoords(mouse->position);

        auto col = static_cast<int>(gridPos.x / CELL_SIZE);
        auto row = static_cast<int>(gridPos.y / CELL_SIZE);

        if (Cell *cell = grid.getCell(col, row))
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                if (firstReveal)
                {
                    mineGenerator.generate(grid, col, row);
                    reveal(col, row);

                    firstReveal = false;

                    return;
                }

                if (cell->getState() == Cell::State::Unrevealed ||
                    cell->getState() == Cell::State::Flagged)
                {
                    if (cell->getType() == Cell::Type::Empty)
                    {
                        reveal(col, row);
                    }
                    else if (cell->getType() == Cell::Type::Mine)
                    {
                        gameLost = true;
                    }
                }
            }
            else if (mouse->button == sf::Mouse::Button::Right)
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
            else if (mouse->button == sf::Mouse::Button::Middle)
            {
                std::cout << *cell << std::endl;
            }
        }
    }
}

std::unique_ptr<GameState> PlayingState::getNextState()
{
    return nullptr;
}

void PlayingState::print(std::ostream &os) const
{
    os << "PlayingState";
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

// TODO: use a stack instead of recursion
void PlayingState::reveal(int col, int row)
{
    Cell *cell = grid.getCell(col, row);
    if (!cell)
    {
        return;
    }

    if (cell->getState() == Cell::State::Revealed)
    {
        return;
    }

    cell->setState(Cell::State::Revealed);

    // NOTE: I think it can even be proved that, here, the current cell is
    // Empty, but I'll keep the check because I'm too tired for this
    if (cell->getType() == Cell::Type::Empty && cell->getMineCount() == 0)
    {
        for (int rowOff = -1; rowOff <= 1; rowOff++)
        {
            for (int colOff = -1; colOff <= 1; colOff++)
            {
                if (colOff == 0 && rowOff == 0)
                {
                    continue;
                }

                reveal(col + colOff, row + rowOff);
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
        return {190, 190, 190};
    case Cell::State::Revealed:
        return {127, 127, 127};
    case Cell::State::Flagged:
        return {255, 255, 0};
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
