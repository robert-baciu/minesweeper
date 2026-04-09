#include "State.hpp"

#include <optional>

std::ostream &operator<<(std::ostream &os, StateContext const &ctx)
{
    os << "StateContext[assets=" << ctx.assets << ", window=" << ctx.window
       << "]";
    return os;
}

State::State(StateContext const &ctx) : ctx(ctx)
{
}

void State::update([[maybe_unused]] double dt)
{
}

void State::handleEvent([[maybe_unused]] std::optional<sf::Event> const &event)
{
}

void State::draw([[maybe_unused]] sf::RenderTarget &target,
                 [[maybe_unused]] sf::RenderStates states) const
{
}

void State::requestExit()
{
    requestedExit = true;
}

std::optional<State::Transition> State::getTransition() const
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    return std::nullopt;
}
std::ostream &operator<<(std::ostream &os, State const &state)
{
    state.print(os);
    return os;
}
