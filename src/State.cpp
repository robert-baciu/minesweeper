#include "State.hpp"

#include <optional>

#include "TGUI/Font.hpp"

State::State(State::Context const &ctx) : ctx{ctx}, gui{ctx.getWindow().get()}
{
    auto font = tgui::Font("assets/fonts/VCR_OSD_MONO_1.001.ttf");
    gui.setFont(font);
    gui.setTextSize(32);
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

tgui::Gui &State::getGui()
{
    return gui;
}

std::ostream &operator<<(std::ostream &os, State const &state)
{
    state.print(os);
    return os;
}

std::ostream &operator<<(std::ostream &os, State::Context const &ctx)
{
    os << "State::Context[assets=" << ctx.getAssets()
       << ", window=" << ctx.getWindow() << "]";
    return os;
}
