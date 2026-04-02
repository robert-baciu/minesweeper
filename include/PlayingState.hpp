#pragma once

#include <ostream>
#include <SFML/Graphics/RenderWindow.hpp>

#include "GameState.hpp"

class PlayingState : public GameState
{
  public:
    void draw(sf::RenderWindow &window) override;
    std::unique_ptr<GameState> getNextState() override;
    void print(std::ostream &os) const override;
    void update(double dt) override;
};

void PlayingState::draw(sf::RenderWindow &window)
{
    window.clear(sf::Color::Green);
}

std::unique_ptr<GameState> PlayingState::getNextState()
{
    return nullptr;
}

void PlayingState::print(std::ostream &os) const
{
    os << "PlayingState";
}

void PlayingState::update([[maybe_unused]] double dt)
{
}
