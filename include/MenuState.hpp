#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameState.hpp"
#include "PlayingState.hpp"

class MenuState : public GameState
{
  public:
    void draw(sf::RenderWindow &window) override;
    std::unique_ptr<GameState> getNextState() override;
    void handleEvent(std::optional<sf::Event> const &event) override;
    void print(std::ostream &os) const override;
    void update(double dt) override;

  private:
    bool startPlaying = false;
};

void MenuState::draw(sf::RenderWindow &window)
{
    window.clear(sf::Color::Red);
}

std::unique_ptr<GameState> MenuState::getNextState()
{
    if (startPlaying)
    {
        return std::make_unique<PlayingState>();
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
