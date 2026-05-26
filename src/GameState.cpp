#include "GameState.hpp"

#include <memory>
#include <utility>

#include "State.hpp"

GameState::GameState(std::shared_ptr<GameStateCtx> gameCtx_)
    : State(std::move(gameCtx_)),
      gameCtx(std::dynamic_pointer_cast<GameStateCtx>(ctx)),
      restart(false)
{
}

void GameState::print(std::ostream &os) const
{
    os << "GameState";
}
