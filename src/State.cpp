#include "State.hpp"

std::ostream &operator<<(std::ostream &os, StateContext const &ctx)
{
    os << "StateContext[assets=" << ctx.assets << ", window=" << ctx.window
       << "]";
    return os;
}

State::State(StateContext const &ctx) : ctx(ctx)
{
}
