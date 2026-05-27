#pragma once

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Mouse.hpp>

#include "GameState.hpp"

class PlayingState : public GameState
{
  public:
    explicit PlayingState(std::unique_ptr<StateCtx> gameCtx_);

    void update(double dt) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

  private:
    void gridLeftClick(int col, int row);
    void gridRightClick(int col, int row);

    void firstReveal(int col, int row);
    void chordingReveal(int col, int row);
    void floodReveal(int col, int row);

    bool lost;
    bool won;

    bool started;
};
