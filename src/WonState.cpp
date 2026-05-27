#include "WonState.hpp"

#include <optional>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "LeaderboardEntry.hpp"
#include "LeaderboardManager.hpp"
#include "MenuState.hpp"
#include "PlayingGrid.hpp"
#include "PlayingState.hpp"

WonState::WonState(std::unique_ptr<StateCtx> gameCtx_)
    : GameState(std::move(gameCtx_))
{
    // TODO: SmileyFace class
    gameCtx->getHeader().setSmiley(
        gameCtx->getAssets().getTexture("smiley-won"));
    gameCtx->getHeader().setRemainingMines(0);

    LeaderboardEntry entry(gameCtx->getDifficulty(),
                           gameCtx->getHeader().getTime());
    LeaderboardManager::save(entry);
}

void WonState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(gameCtx->getHeader(), states);
    target.draw(gameCtx->getGrid(), states);

    states.transform.translate(gameCtx->getGrid().getPosition());

    sf::RectangleShape highlightRect = PlayingGrid::getCellRect();

    gameCtx->getGrid().all(
        [&](int col, int row)
        {
            Cell const *cell = gameCtx->getGrid().getCell(col, row);

            if (cell->isMine())
            {
                sf::Vector2f cellPos = PlayingGrid::CELL_SIZE *
                                       sf::Vector2f(sf::Vector2i(col, row));

                sf::RenderStates cellStates = states;
                cellStates.transform.translate(cellPos);

                if (cell->isFlagged())
                {
                    highlightRect.setFillColor(FLAG_CORRECT_COLOR);
                    target.draw(highlightRect, cellStates);
                }

                target.draw(gameCtx->getGrid().getMineSprite(), cellStates);
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

void WonState::print(std::ostream &os) const
{
    os << "WonState";
}
