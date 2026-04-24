#include "LostState.hpp"

#include <memory>
#include <optional>

#include "MenuState.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

LostState::LostState(State::Context const &ctx) : State(ctx)
{
}

void LostState::handleEvent(std::optional<sf::Event> const &event)
{
    if (event->is<sf::Event::KeyPressed>())
    {
        auto const *key = event->getIf<sf::Event::KeyPressed>();
        if (key->scancode == sf::Keyboard::Scancode::Enter)
        {
            backToMainMenu = true;
        }
    }
}

void LostState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f{target.getSize()});
    overlay.setFillColor(sf::Color{51, 0, 0, 153});
    overlay.setPosition({0, 0});

    sf::Text text{ctx.getAssets().getMainFont(), "YOU LOST!", 64};
    sf::FloatRect textRect = text.getLocalBounds();
    float textScale = std::min(1.0f, static_cast<float>(target.getSize().x) *
                                         0.8f / textRect.size.x);
    text.setScale({textScale, textScale});

    text.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                    textRect.position.y + textRect.size.y / 2.0f});

    text.setPosition({static_cast<float>(target.getSize().x) / 2.0f,
                      static_cast<float>(target.getSize().y) / 3.0f});

    sf::Text subtext{ctx.getAssets().getMainFont(),
                     "Press Enter to go back to main menu", 64};
    sf::FloatRect subtextRect = subtext.getLocalBounds();

    float subtextScale = std::min(1.0f, static_cast<float>(target.getSize().x) *
                                            0.8f / subtextRect.size.x);
    subtext.setScale({subtextScale, subtextScale});

    subtext.setOrigin({subtextRect.position.x + subtextRect.size.x / 2.0f,
                       subtextRect.position.y + subtextRect.size.y / 2.0f});

    subtext.setPosition({static_cast<float>(target.getSize().x) / 2.0f,
                         static_cast<float>(target.getSize().y) * 2.0f / 3.0f});

    target.draw(overlay, states);
    target.draw(text, states);
    target.draw(subtext, states);
}

std::optional<State::Transition> LostState::getTransition() const
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (backToMainMenu)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<MenuState>(ctx);
        return transition;
    }

    return std::nullopt;
}

void LostState::print(std::ostream &os) const
{
    os << "WinState";
}
