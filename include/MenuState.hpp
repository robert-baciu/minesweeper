#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <TGUI/Widgets/Button.hpp>

#include "State.hpp"

class MenuState : public State
{
  public:
    explicit MenuState(State::Context const &ctx);

    [[nodiscard]] std::optional<Transition> getTransition() const override;

    void print(std::ostream &os) const override;

  private:
    bool requestedPlay = false;
};
