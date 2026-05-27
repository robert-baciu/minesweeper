#include "PlayingState.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "LostState.hpp"
#include "WonState.hpp"

PlayingState::PlayingState(std::unique_ptr<StateCtx> gameCtx_)
    : GameState(std::move(gameCtx_)),
      lost(false),
      won(false),
      started(false)
{
}

void PlayingState::update([[maybe_unused]] double dt)
{
    gameCtx->getHeader().setRemainingMines(gameCtx->getGrid().getMineCount() -
                                           gameCtx->getGrid().getFlagCount());
    gameCtx->getHeader().update();

    if (!started)
    {
        if (gameCtx->getGrid().isStarted())
        {
            gameCtx->getHeader().startClock();
            started = true;
        }
    }

    if (gameCtx->getHeader().shouldRestart())
    {
        restart = true;
    }

    PlayingGrid::GridState gridState = gameCtx->getGrid().getState();
    if (gridState == PlayingGrid::GridState::Lost)
    {
        lost = true;
    }
    else if (gridState == PlayingGrid::GridState::Won)
    {
        won = true;
    }
}

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(gameCtx->getHeader(), states);
    target.draw(gameCtx->getGrid(), states);
}

std::optional<State::Transition> PlayingState::getTransition()
{
    // TODO: Redesign
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

    if (lost)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<LostState>(std::move(ctx));
        return transition;
    }

    if (won)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<WonState>(std::move(ctx));
        return transition;
    }

    return std::nullopt;
}

void PlayingState::print(std::ostream &os) const
{
    os << "PlayingState";
}
