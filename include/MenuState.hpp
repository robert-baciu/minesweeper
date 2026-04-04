#pragma once

#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameState.hpp"
#include "PlayingState.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "StateContext.hpp"

class MenuState : public GameState
{
  public:
    explicit MenuState(StateContext const &ctx);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    std::unique_ptr<GameState> getNextState() override;
    void handleEvent(std::optional<sf::Event> const &event) override;
    void print(std::ostream &os) const override;
    void update(double dt) override;

  private:
    bool startPlaying = false;
};

MenuState::MenuState(StateContext const &ctx) : GameState(ctx)
{
}

void MenuState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto text = sf::Text{ctx.assets.getFont("main"), "Press ENTER to play", 32};
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.getCenter());
    text.setPosition(target.getView().getCenter());
    target.draw(text, states);
}

std::unique_ptr<GameState> MenuState::getNextState()
{
    if (startPlaying)
    {
        return std::make_unique<PlayingState>(ctx);
    }
    return nullptr;
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

void MenuState::print(std::ostream &os) const
{
    os << "MenuState";
}

void MenuState::update([[maybe_unused]] double dt)
{
}
