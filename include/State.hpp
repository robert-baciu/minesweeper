#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "AssetManager.hpp"
#include "GameWindow.hpp"

struct StateContext
{
    AssetManager const &assets;
    GameWindow &window;

    explicit StateContext(AssetManager const &assets, GameWindow &window)
        : assets(assets), window(window)
    {
    }
};

std::ostream &operator<<(std::ostream &os, StateContext const &ctx);

class State : public sf::Drawable
{
  public:
    enum class Action
    {
        Change,
        /* Push,
        Pop, */
        Exit
    };

    struct Transition
    {
        Action action;
        std::unique_ptr<State> state;
    };

    explicit State(StateContext const &ctx);

    virtual void update(double dt) = 0;
    virtual void handleEvent(std::optional<sf::Event> const &)
    {
    }

    virtual std::optional<Transition> getTransition() const = 0;

    virtual void requestExit() = 0;

    friend std::ostream &operator<<(std::ostream &os, State const &state)
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
