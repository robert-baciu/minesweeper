#pragma once

#include "State.hpp"

class LeaderboardState : public State
{
  public:
    explicit LeaderboardState(StateCtxPtr ctx_);

    void print(std::ostream &os) const override;
};
