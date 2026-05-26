#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameStateCtx.hpp"
#include "State.hpp"

class GameState : public State
{
  public:
    // TODO: Move into own file

    explicit GameState(GameStateCtxPtr gameCtx_);

    void print(std::ostream &os) const override;

  protected:
    GameStateCtxPtr gameCtx;

    bool restart;
};
