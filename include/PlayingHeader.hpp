#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

#include "PlayingGrid.hpp"
#include "StateCtx.hpp"

class PlayingHeader : public sf::Drawable
{
  public:
    PlayingHeader(StateCtx const &ctx, sf::Vector2f size,
                  sf::Vector2f position);

    void handleEvent(std::optional<sf::Event> const &event);

    void update();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void startClock();
    float getTime() const;

    void setRemainingMines(unsigned int remaining);

    void setSmiley(sf::Texture const &texture);

    bool shouldRestart() const;

    sf::Vector2f getSize() const;
    sf::Vector2f getPosition() const;

  private:
    static constexpr sf::Color HEADER_COLOR{23, 23, 23};
    static constexpr sf::Color TEXT_COLOR{240, 240, 240};

    static constexpr unsigned int TEXT_SIZE = 48.0f;

    static constexpr sf::Vector2f SMILEY_SIZE{1.5f * PlayingGrid::CELL_SIZE,
                                              1.5f * PlayingGrid::CELL_SIZE};

    StateCtx const &ctx;

    sf::RectangleShape header;
    sf::Vector2f position;
    sf::View view;

    sf::Clock clock;
    sf::Text clockText;

    unsigned int remaining;
    sf::Text remainingText;

    sf::Sprite smiley;

    bool restart;
};
