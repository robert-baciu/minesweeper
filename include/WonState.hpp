#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "State.hpp"

class WonState : public State
{
  public:
    explicit WonState(State::Context const &ctx);

    virtual void handleEvent(std::optional<sf::Event> const &event) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() const override;

    void print(std::ostream &os) const override;

  private:
    bool backToMainMenu = false;
};
