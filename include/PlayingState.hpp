#pragma once

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "CellGrid.hpp"
#include "DifficultySettings.hpp"
#include "PlayingHeader.hpp"
#include "State.hpp"

class PlayingState : public State
{
  public:
    explicit PlayingState(State::Context const &ctx,
                          DifficultySettings settings);

    void handleEvent(std::optional<sf::Event> const &event) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void update(double dt) override;

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

    enum class Status
    {
        Ongoing,
        Lost,
        Won
    };

  private:
    void gridLeftClick(int col, int row);
    void gridRightClick(int col, int row);

    void firstReveal(int col, int row);
    void chordingReveal(int col, int row);
    void floodReveal(int col, int row);

    static constexpr float HEADER_H = 2 * CellGrid::CELL_SIZE;

    static constexpr auto PADDING =
        sf::Vector2f(0.5f * CellGrid::CELL_SIZE, 0.5f * CellGrid::CELL_SIZE);

    int cols;
    int rows;

    sf::Vector2f gridSize;

    sf::Vector2f winSizeFloat;
    sf::Vector2u winSize;

    sf::View headerView;
    sf::View gridView;

    PlayingHeader header;
    CellGrid grid;

    unsigned int cellCount;
    unsigned int mineCount;
    unsigned int revealedCount;
    unsigned int flagCount;

    Status playingStatus;

    sf::Vector2i detonatedPos;
};
