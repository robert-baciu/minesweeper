#pragma once

#include <ostream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "GameState.hpp"

class QuittingState : public GameState
{
  public:
    void draw(sf::RenderWindow &window) override;
    std::unique_ptr<GameState> getNextState() override;
    bool isReadyToExit() override;
    void print(std::ostream &os) const override;
    void update(double dt) override;

  private:
    bool readyToExit = true;
};

void QuittingState::draw(sf::RenderWindow &window)
{
    window.clear(sf::Color::White);
}

std::unique_ptr<GameState> QuittingState::getNextState()
{
    return nullptr;
}

bool QuittingState::isReadyToExit()
{
    return readyToExit;
}

void QuittingState::print(std::ostream &os) const
{
    os << "QuittingState[readyToExit=" << readyToExit << "]";
}

void QuittingState::update([[maybe_unused]] double dt)
{
}
