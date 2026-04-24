#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "AssetManager.hpp"
#include "GameWindow.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

class State : public sf::Drawable
{
  public:
    class Context
    {
      public:
        explicit Context(AssetManager const &assets, GameWindow &window)
            : assets(assets), window(window)
        {
        }

        AssetManager const &getAssets() const
        {
            return assets;
        }

        GameWindow &getWindow() const
        {
            return window;
        }

      private:
        AssetManager const &assets;
        GameWindow &window;
    };

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

    explicit State(State::Context const &ctx);

    virtual void update(double dt);

    virtual void handleEvent(std::optional<sf::Event> const &event);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] virtual std::optional<Transition> getTransition() const;

    virtual void requestExit();

    tgui::Gui &getGui();

    virtual void print(std::ostream &os) const = 0;

    friend std::ostream &operator<<(std::ostream &os, State const &state);

  protected:
    State::Context const &ctx;

    tgui::Gui gui;

    bool requestedExit = false;
};

std::ostream &operator<<(std::ostream &os, State::Context const &ctx);
