#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameStateCtx.hpp"
#include "State.hpp"

class GameState : public State
{
  public:
    explicit GameState(std::unique_ptr<StateCtx> gameCtx_);

    void handleEvent(std::optional<sf::Event> const &event) override;

    void update(double dt) override;

    void print(std::ostream &os) const override;

  protected:
    GameStateCtx *gameCtx;

    bool pause;
    bool restart;
};
