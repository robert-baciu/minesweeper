#pragma once

#include "GameState.hpp"

class PausedState : public GameState
{
  public:
    explicit PausedState(std::unique_ptr<StateCtx> gameCtx_);

    [[nodiscard]] std::optional<Transition> getTransition() override;

  private:
    bool resume;
    bool gotoMenu;
};
