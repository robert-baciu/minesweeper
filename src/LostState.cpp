#include "LostState.hpp"

#include <cmath>
#include <memory>
#include <optional>
#include <SFML/Graphics/RenderStates.hpp>

#include "MenuState.hpp"

LostState::LostState(State::Context const &ctx, CellGrid grid,
                     sf::Vector2i detonatedPos)
    : State{ctx}, grid{std::move(grid)}, detonatedPos{detonatedPos},
      flagSprite{ctx.getAssets().getTexture("flag")}
{
    overlay.setSize(sf::Vector2f{ctx.getWindow().get().getSize()});
}

void LostState::handleEvent(std::optional<sf::Event> const &event)
{
    if (event->is<sf::Event::KeyPressed>())
    {
        auto const *key = event->getIf<sf::Event::KeyPressed>();
        if (key->scancode == sf::Keyboard::Scancode::Enter)
        {
            transitionToMenu = true;
        }
    }
}

void LostState::update(double dt)
{
    totalTime += dt;

    float const speed = 5.0f;
    auto const wave = static_cast<float>(std::sin(totalTime * speed));
    float const normalized = (wave + 1.0f) / 2.0f;
    auto const alpha = static_cast<uint8_t>(0.0f + (normalized * 31.0f));

    overlay.setFillColor({255, 0, 0, alpha});
}

void LostState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    target.draw(grid, states);

    sf::RectangleShape highlight{
        {CellGrid::CELL_SIZE - CellGrid::CELL_PADDING,
         CellGrid::CELL_SIZE - CellGrid::CELL_PADDING}};

    grid.all(
        [&](int col, int row)
        {
            Cell const *cell = grid.getCell(col, row);
            auto cellPos =
                sf::Vector2f{sf::Vector2i{col, row}} * CellGrid::CELL_SIZE;
            sf::RenderStates cellStates = states;
            cellStates.transform.translate(cellPos);

            if (!cell->isMine())
            {
                if (cell->isFlagged())
                {
                    highlight.setFillColor(FLAG_MISPLACE_COLOR);
                    target.draw(highlight, cellStates);
                    target.draw(flagSprite, cellStates);
                }

                return;
            }

            if (cell->isFlagged())
            {
                highlight.setFillColor(MINE_FLAGGED_COLOR);
                target.draw(highlight, cellStates);
            }

            if (sf::Vector2i{col, row} == detonatedPos)
            {
                highlight.setFillColor(MINE_DETONATED_COLOR);
                target.draw(highlight, cellStates);
            }

            float const mineRadius =
                (CellGrid::CELL_SIZE - CellGrid::CELL_PADDING) / 3.0f;
            sf::CircleShape mine{mineRadius};
            mine.setOrigin({mineRadius, mineRadius});
            mine.setPosition(
                {CellGrid::CELL_SIZE / 2.0f, CellGrid::CELL_SIZE / 2.0f});
            mine.setFillColor(MINE_COLOR);
            target.draw(mine, cellStates);
        });

    target.draw(overlay, states);
}

std::optional<State::Transition> LostState::getTransition()
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (transitionToMenu)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<MenuState>(ctx);
        return transition;
    }

    return std::nullopt;
}

void LostState::print(std::ostream &os) const
{
    os << "LostState";
}
