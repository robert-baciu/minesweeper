#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "StateContext.hpp"

class GameState : public sf::Drawable
{
  public:
    explicit GameState(StateContext const &ctx);

    virtual void update(double dt) = 0;
    virtual void handleEvent(std::optional<sf::Event> const &)
    {
    }

    virtual std::unique_ptr<GameState> getNextState() = 0;
    virtual std::unique_ptr<GameState> getExitState()
    {
        return nullptr;
    }

    virtual bool isReadyToExit()
    {
        return false;
    }

    friend std::ostream &operator<<(std::ostream &os, GameState const &state)
    {
        state.print(os);
        return os;
    }

  protected:
    void draw(sf::RenderTarget &target,
              sf::RenderStates states) const override = 0;

    virtual void print(std::ostream &os) const = 0;

    StateContext const &ctx;
};
