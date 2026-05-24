#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>

#include "CellGrid.hpp"
#include "State.hpp"

class PlayingHeader : public sf::Drawable
{
  public:
    explicit PlayingHeader(State::Context const &ctx);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void handleEvent(std::optional<sf::Event> const &event);

    void update();

    void startClock();

    sf::Sprite const &getSmiley();

    void setHeaderSize(sf::Vector2f size);
    void setRemainingMines(int remaining);
    void setSmiley(sf::Texture const &texture);

  private:
    static constexpr sf::Color HEADER_COLOR{23, 23, 23};
    static constexpr sf::Color TEXT_COLOR{240, 240, 240};

    static constexpr sf::Vector2f SMILEY_SIZE{1.5f * CellGrid::CELL_SIZE,
                                              1.5f * CellGrid::CELL_SIZE};

    sf::RectangleShape header;

    sf::Clock clock;
    sf::Text clockText;

    int remainingMines;
    sf::Text remainingText;

    sf::Sprite smiley;
};
