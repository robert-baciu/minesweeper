#pragma once

#include <ostream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "GameState.hpp"
#include "StateContext.hpp"

class ExitingState : public GameState
{
  public:
    explicit ExitingState(StateContext const &ctx);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    std::unique_ptr<GameState> getNextState() override;
    bool isReadyToExit() override;
    void print(std::ostream &os) const override;
    void update(double dt) override;

  private:
    bool readyToExit = true;
};

ExitingState::ExitingState(StateContext const &ctx) : GameState(ctx)
{
}

void ExitingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape bg;
    bg.setSize(target.getView().getSize());
    bg.setFillColor(sf::Color::White);
    target.draw(bg, states);
}

std::unique_ptr<GameState> ExitingState::getNextState()
{
    return nullptr;
}

bool ExitingState::isReadyToExit()
{
    return readyToExit;
}

void ExitingState::print(std::ostream &os) const
{
    os << "ExitingState[readyToExit=" << readyToExit << "]";
}

void ExitingState::update([[maybe_unused]] double dt)
{
}
