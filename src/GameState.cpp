#include "GameState.hpp"

#include <memory>

#include "InvalidCtxError.hpp"
#include "PausedState.hpp"
#include "State.hpp"

GameState::GameState(std::unique_ptr<StateCtx> gameCtx_)
    : State(std::move(gameCtx_)),
      gameCtx(dynamic_cast<GameStateCtx *>(ctx.get())),

      pause(false),
      restart(false)
{
    if (gameCtx == nullptr)
    {
        throw InvalidStateCtxError(
            "A GameState must be initialized with a valid GameStateCtx");
    }
}

void GameState::handleEvent(std::optional<sf::Event> const &event)
{
    if (!dynamic_cast<PausedState *>(this))
    {
        gameCtx->getHeader().handleEvent(event);
        gameCtx->getGrid().handleEvent(event);

        if (event->is<sf::Event::KeyPressed>())
        {
            auto const *keyPressed = event->getIf<sf::Event::KeyPressed>();
            if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                pause = true;
            }
        }
    }
}

void GameState::update([[maybe_unused]] double dt)
{
    if (gameCtx->getHeader().shouldRestart())
    {
        restart = true;
    }
}

void GameState::print(std::ostream &os) const
{
    os << "GameState";
}
