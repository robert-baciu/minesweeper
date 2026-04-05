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

    void update(double dt) override;

    std::unique_ptr<GameState> getNextState() override;

    bool isReadyToExit() override;

    void print(std::ostream &os) const override;

  private:
    bool readyToExit = true;
};
