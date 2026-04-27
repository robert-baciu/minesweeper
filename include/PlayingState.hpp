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
#include "RandomMineGenerator.hpp"
#include "State.hpp"

class PlayingState : public State
{
  public:
    explicit PlayingState(State::Context const &ctx, int cols, int rows);

    void handleEvent(std::optional<sf::Event> const &event) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() const override;

    void print(std::ostream &os) const override;

    enum class Status
    {
        Ongoing,
        Lost,
        Won
    };

  private:
    static constexpr float CELL_SIZE = 64.0f;
    static constexpr float CELL_PADDING = 2.0f;
    static constexpr auto CELL_TEXT_CHAR_SIZE =
        static_cast<unsigned int>(CELL_SIZE * 0.9f);

    void onCellLeftClick(Cell *cell, int col, int row);
    static void onCellRightClick(Cell *cell);

    void chordingReveal(Cell *cell, int col, int row);
    void floodReveal(int startCol, int startRow);

    static sf::Color getCellColor(Cell::State state);
    static sf::Color getCellTextColor(unsigned int mineCount);

    int cols;
    int rows;

    CellGrid grid;

    unsigned int totalMineCells;
    unsigned int totalSafeCells;
    unsigned int revealedCellCount;

    mutable sf::RectangleShape cellShape;
    mutable sf::Text cellText;

    Status playingStatus = Status::Ongoing;
};
