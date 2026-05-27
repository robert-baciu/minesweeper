#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameState.hpp"

class WonState : public GameState
{
  public:
    explicit WonState(std::unique_ptr<StateCtx> gameCtx_);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

  private:
    static constexpr sf::Color FLAG_CORRECT_COLOR{0, 190, 0};

    bool gotoMenu = false;
};
