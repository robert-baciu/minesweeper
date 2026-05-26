#include "LostState.hpp"

#include <memory>
#include <optional>
#include <SFML/Graphics/RenderStates.hpp>

#include "MenuState.hpp"

LostState::LostState(GameStateCtxPtr gameCtx_, sf::Vector2i detonated_)
    : GameState(std::move(gameCtx_)),
      detonated(detonated_)
{
    gameCtx->getHeader().setSmiley(
        gameCtx->getAssets().getTexture("smiley-lost"));
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
    target.setView(gameCtx->getHeaderView());
    target.draw(gameCtx->getHeader(), states);

    target.setView(gameCtx->getGridView());
    target.draw(gameCtx->getGrid(), states);

    sf::RectangleShape highlight{
        {PlayingGrid::CELL_SIZE - PlayingGrid::CELL_PADDING,
         PlayingGrid::CELL_SIZE - PlayingGrid::CELL_PADDING}};

    gameCtx->getGrid().all(
        [&](int col, int row)
        {
            Cell const *cell = gameCtx->getGrid().getCell(col, row);
            auto cellPos =
                sf::Vector2f{sf::Vector2i{col, row}} * PlayingGrid::CELL_SIZE;
            sf::RenderStates cellStates = states;
            cellStates.transform.translate(cellPos);

            if (!cell->isMine())
            {
                if (cell->isFlagged())
                {
                    highlight.setFillColor(FLAG_MISPLACE_COLOR);
                    target.draw(highlight, cellStates);
                    target.draw(gameCtx->getGrid().getFlagSprite(), cellStates);
                }

                return;
            }

            if (cell->isFlagged())
            {
                highlight.setFillColor(MINE_FLAGGED_COLOR);
                target.draw(highlight, cellStates);
            }

            if (sf::Vector2i{col, row} == detonated)
            {
                highlight.setFillColor(MINE_DETONATED_COLOR);
                target.draw(highlight, cellStates);
            }

            target.draw(gameCtx->getGrid().getMineSprite(), cellStates);
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
        transition.state = std::make_unique<MenuState>(std::move(ctx));
        return transition;
    }

    return std::nullopt;
}

void LostState::print(std::ostream &os) const
{
    os << "LostState";
}
