#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class GameState
{
  public:
    virtual ~GameState() = default;

    friend std::ostream &operator<<(std::ostream &os, GameState const &state)
    {
        state.print(os);
        return os;
    }

    virtual void draw(sf::RenderWindow &window) = 0;
    virtual std::unique_ptr<GameState> getNextState() = 0;
    virtual void update(double dt) = 0;

    virtual std::unique_ptr<GameState> getExitState()
    {
        return nullptr;
    }

    virtual void handleEvent(std::optional<sf::Event> const &)
    {
    }

    virtual bool isReadyToExit()
    {
        return false;
    }

    virtual std::unique_ptr<GameState> onQuitRequested()
    {
        return nullptr;
    }

  protected:
    virtual void print(std::ostream &os) const = 0;
};
