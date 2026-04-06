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

#include "CellGrid.hpp"
#include "GameState.hpp"
#include "RandomMineGenerator.hpp"

class PlayingState : public GameState
{
  public:
    explicit PlayingState(StateContext const &ctx, int cols, int rows);

    void update(double dt) override;
    void handleEvent(std::optional<sf::Event> const &event) override;

    std::unique_ptr<GameState> getNextState() override;

    void print(std::ostream &os) const override;

  protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    static constexpr float CELL_SIZE = 64.0f;
    static constexpr float CELL_PADDING = 2.0f;
    static constexpr auto CELL_TEXT_CHAR_SIZE =
        static_cast<unsigned int>(CELL_SIZE * 0.9f);

    static constexpr unsigned int START_SAFE_DISTANCE = 2;

    void reveal(int col, int row);

    unsigned int getMineCount() const;

    static sf::Color getCellColor(Cell::State state);
    static sf::Color getCellTextColor(unsigned int mineCount);

    int cols;
    int rows;

    CellGrid grid;

    RandomMineGenerator mineGenerator;

    mutable sf::RectangleShape cellShape;
    mutable sf::Text cellText;

    bool gameLost = false;
    bool firstReveal = true;
};
