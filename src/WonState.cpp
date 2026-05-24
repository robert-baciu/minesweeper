#include "WonState.hpp"

#include <optional>
#include <SFML/System/Vector2.hpp>

#include "MenuState.hpp"
#include "SFML/Graphics/Color.hpp"

WonState::WonState(State::Context const &ctx, WindowLayout layout)
    : State{ctx},
      layout(std::move(layout)),
      flagSprite(ctx.getAssets().getTexture("flag")),
      mineSprite(ctx.getAssets().getTexture("mine"))
{
    this->layout.header.setSmiley(ctx.getAssets().getTexture("smiley-won"));
    this->layout.header.setRemainingMines(0);
}

void WonState::handleEvent(std::optional<sf::Event> const &event)
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

void WonState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.setView(layout.headerView);
    target.draw(layout.header, states);

    target.setView(layout.gridView);
    target.draw(layout.grid, states);

    sf::RectangleShape highlight{
        {CellGrid::CELL_SIZE - CellGrid::CELL_PADDING,
         CellGrid::CELL_SIZE - CellGrid::CELL_PADDING}};

    layout.grid.all(
        [&](int col, int row)
        {
            Cell const *cell = layout.grid.getCell(col, row);

            if (cell->isMine())
            {
                auto cellPos =
                    sf::Vector2f{sf::Vector2i{col, row}} * CellGrid::CELL_SIZE;
                sf::RenderStates cellStates = states;
                cellStates.transform.translate(cellPos);

                if (cell->isFlagged())
                {

                    highlight.setFillColor(sf::Color(0, 190, 0));
                    target.draw(highlight, cellStates);
                    target.draw(mineSprite, cellStates);
                }
                else
                {
                    auto cellPos = sf::Vector2f{sf::Vector2i{col, row}} *
                                   CellGrid::CELL_SIZE;
                    sf::RenderStates cellStates = states;
                    cellStates.transform.translate(cellPos);

                    target.draw(mineSprite, cellStates);
                }
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

    if (transitionToMenu)
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
