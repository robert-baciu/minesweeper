#include "MenuState.hpp"

#include <memory>

#include "PlayingState.hpp"
#include "State.hpp"

MenuState::MenuState(StateContext const &ctx) : State(ctx)
{
}

void MenuState::update([[maybe_unused]] double dt)
{
}

void MenuState::handleEvent(std::optional<sf::Event> const &event)
{
    if (event->is<sf::Event::KeyPressed>())
    {
        auto const *key = event->getIf<sf::Event::KeyPressed>();
        if (key->scancode == sf::Keyboard::Scancode::Enter)
        {
            startPlaying = true;
        }
    }
}

void MenuState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto text = sf::Text{ctx.assets.getMainFont(), "Press ENTER to play", 32};
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.getCenter());
    text.setPosition(target.getView().getCenter());
    target.draw(text, states);
}

std::optional<State::Transition> MenuState::getTransition() const
{
    if (startPlaying)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<PlayingState>(ctx, 16, 24);
        return transition;
    }

    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    return std::nullopt;
}

void MenuState::requestExit()
{
    requestedExit = true;
}

void MenuState::print(std::ostream &os) const
{
    os << "MenuState";
}
