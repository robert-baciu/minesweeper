#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "State.hpp"

class MenuState : public State
{
  public:
    explicit MenuState(StateContext const &ctx);

    void update(double dt) override;
    void handleEvent(std::optional<sf::Event> const &event) override;

    std::optional<Transition> getTransition() const override;

    void requestExit() override;

    void print(std::ostream &os) const override;

  protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    bool startPlaying = false;
    bool requestedExit = false;
};
