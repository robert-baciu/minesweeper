#include "State.hpp"

#include <optional>
#include <TGUI/Font.hpp>

State::State(std::unique_ptr<StateCtx> ctx_)
    : ctx(std::move(ctx_)),
      requestedExit(false)
{
    ctx->getWindow().getGui().removeAllWidgets();
}

void State::handleEvent([[maybe_unused]] std::optional<sf::Event> const &event)
{
}

void State::update([[maybe_unused]] double dt)
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

std::optional<State::Transition> State::getTransition()
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
