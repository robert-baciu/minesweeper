#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include "StateCtx.hpp"

class State : public sf::Drawable
{
  public:
    // TODO: Transition::Action
    enum class Action
    {
        Change,
        Push,
        Pop,
        Exit
    };

    struct Transition
    {
        Action action;
        std::unique_ptr<State> state;
    };

    explicit State(std::unique_ptr<StateCtx> ctx_);

    ~State() override = default;

    virtual void handleEvent(std::optional<sf::Event> const &event);

    virtual void update(double dt);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] virtual std::optional<Transition> getTransition();

    virtual void requestExit();

    virtual void print(std::ostream &os) const = 0;

    friend std::ostream &operator<<(std::ostream &os, State const &state);

  protected:
    std::unique_ptr<StateCtx> ctx;

    bool requestedExit;
};
