#include "LostState.hpp"

#include <memory>
#include <optional>
#include <SFML/Graphics/RenderStates.hpp>

#include "MenuState.hpp"
#include "WindowLayout.hpp"

LostState::LostState(State::Context const &ctx, WindowLayout layout,
                     sf::Vector2i detonatedPos)
    : State{ctx},
      layout(std::move(layout)),
      detonatedPos{detonatedPos},
      flagSprite{ctx.getAssets().getTexture("flag")},
      mineSprite(ctx.getAssets().getTexture("mine"))
{
    this->layout.header.setSmiley(ctx.getAssets().getTexture("smiley-lost"));
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

void LostState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.setView(layout.headerView);
    target.draw(layout.header, states);

    std::cout << "Header drawn\n";

    target.setView(layout.gridView);
    target.draw(layout.grid, states);

    sf::RectangleShape highlight{
        {CellGrid::CELL_SIZE - CellGrid::CELL_PADDING,
         CellGrid::CELL_SIZE - CellGrid::CELL_PADDING}};

    layout.grid.all(
        [&](int col, int row)
        {
            Cell const *cell = layout.grid.getCell(col, row);
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

            target.draw(mineSprite, cellStates);
        });
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
