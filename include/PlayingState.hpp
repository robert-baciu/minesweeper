#pragma once

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "GameState.hpp"
#include "Grid.hpp"

class PlayingState : public GameState
{
  public:
    explicit PlayingState(StateContext const &ctx);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    std::unique_ptr<GameState> getNextState() override;
    void handleEvent(std::optional<sf::Event> const &event) override;
    void print(std::ostream &os) const override;
    void update(double dt) override;

  private:
    static constexpr auto HEADER_HEIGHT = 200.0f;
    static constexpr auto GRID_OFFSET = sf::Vector2f{0.0f, HEADER_HEIGHT};

    static constexpr int COLS = 16;
    static constexpr int ROWS = 24;
    static constexpr float CELL_SIZE = 48.0f;

    static constexpr auto CELL_COLOR = sf::Color{190, 190, 190};
    static constexpr auto CELL_COLOR_HOVER = sf::Color{205, 205, 205};

    Grid grid;
};
