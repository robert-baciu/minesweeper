#include "WonState.hpp"

#include <optional>
#include <SFML/System/Vector2.hpp>

#include "MenuState.hpp"

WonState::WonState(State::Context const &ctx, CellGrid grid)
    : State(ctx), grid(std::move(grid))
{
}

void WonState::handleEvent(std::optional<sf::Event> const &event)
{
    if (event->is<sf::Event::KeyPressed>())
    {
        auto const *key = event->getIf<sf::Event::KeyPressed>();
        if (key->scancode == sf::Keyboard::Scancode::Enter)
        {
            backToMenu = true;
        }
    }
}

void WonState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(grid, states);

    grid.all(
        [&](int col, int row)
        {
            Cell const *cell = grid.getCell(col, row);

            if (cell->isMine() && !cell->isFlagged())
            {
                auto cellPos =
                    sf::Vector2f{sf::Vector2i{col, row}} * CellGrid::CELL_SIZE;
                sf::RenderStates cellStates = states;
                cellStates.transform.translate(cellPos);

                float const mineRadius =
                    (CellGrid::CELL_SIZE - CellGrid::CELL_PADDING) / 3.0f;
                sf::CircleShape mine{mineRadius};
                mine.setOrigin({mineRadius, mineRadius});
                mine.setPosition(
                    {CellGrid::CELL_SIZE / 2.0f, CellGrid::CELL_SIZE / 2.0f});
                mine.setFillColor(MINE_COLOR);
                target.draw(mine, cellStates);
            }
        });
}

std::optional<State::Transition> WonState::getTransition()
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (backToMenu)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<MenuState>(ctx);
        return transition;
    }

    return std::nullopt;
}

void WonState::print(std::ostream &os) const
{
    os << "WonState";
}
