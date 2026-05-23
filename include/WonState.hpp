#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "CellGrid.hpp"
#include "State.hpp"

class WonState : public State
{
  public:
    explicit WonState(State::Context const &ctx, CellGrid grid);

    void handleEvent(std::optional<sf::Event> const &event) override;

    void update(double dt) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

  private:
    static constexpr sf::Color MINE_COLOR = {15, 15, 15};

    CellGrid grid;

    bool transitionToMenu = false;

    double revealTime = 0.0f;
};
