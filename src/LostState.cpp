#include "LostState.hpp"

#include <memory>
#include <optional>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "MenuState.hpp"
#include "PlayingGrid.hpp"
#include "PlayingState.hpp"

LostState::LostState(std::unique_ptr<StateCtx> gameCtx_)
    : GameState(std::move(gameCtx_)),
      gotoMenu(false)
{
    gameCtx->getHeader().setSmiley(
        gameCtx->getAssets().getTexture("smiley-lost"));
}

void LostState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(gameCtx->getHeader(), states);
    target.draw(gameCtx->getGrid(), states);

    states.transform.translate(gameCtx->getGrid().getPosition());

    sf::RectangleShape highlightRect = PlayingGrid::getCellRect();

    gameCtx->getGrid().all(
        [&](int col, int row)
        {
            Cell const *cell = gameCtx->getGrid().getCell(col, row);

            sf::Vector2f cellPos =
                PlayingGrid::CELL_SIZE * sf::Vector2f(sf::Vector2i(col, row));

            sf::RenderStates cellStates = states;
            cellStates.transform.translate(cellPos);

            if (!cell->isMine())
            {
                if (cell->isFlagged())
                {
                    highlightRect.setFillColor(FLAG_WRONG_COLOR);
                    target.draw(highlightRect, cellStates);
                    target.draw(gameCtx->getGrid().getFlagSprite(), cellStates);
                }
            }
            else
            {
                if (cell->isFlagged())
                {
                    highlightRect.setFillColor(MINE_FLAGGED_COLOR);
                    target.draw(highlightRect, cellStates);
                }

                if (sf::Vector2i(col, row) ==
                    gameCtx->getGrid().getDetonatedPos())
                {
                    highlightRect.setFillColor(MINE_DETONATED_COLOR);
                    target.draw(highlightRect, cellStates);
                }

                target.draw(gameCtx->getGrid().getMineSprite(), cellStates);
            }
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

    if (restart)
    {
        auto newGameCtx = std::make_unique<GameStateCtx>(
            *ctx, gameCtx->getDifficulty(), gameCtx->getParams());

        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state =
            std::make_unique<PlayingState>(std::move(newGameCtx));
        return transition;
    }

    if (gotoMenu)
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
