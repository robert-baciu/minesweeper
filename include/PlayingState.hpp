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
#include "PlayingHeader.hpp"
#include "PlaySettings.hpp"
#include "State.hpp"

class PlayingState : public State
{
  public:
    explicit PlayingState(State::Context const &ctx, PlaySettings settings);

    void handleEvent(std::optional<sf::Event> const &event) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

    enum class Status
    {
        Ongoing,
        Lost,
        Won
    };

  private:
    void onCellLeftClick(int col, int row);
    void onCellRightClick(int col, int row);

    void onFirstReveal(int col, int row);
    void chordingReveal(int col, int row);
    void floodReveal(int col, int row);

    int cols;
    int rows;

    CellGrid grid;

    unsigned int totalCellCount;
    unsigned int mineCellCount;
    unsigned int safeCellCount;
    unsigned int revealedCellCount;

    Status playingStatus = Status::Ongoing;

    sf::Vector2i detonatedPos;

    PlayingHeader header;
};
