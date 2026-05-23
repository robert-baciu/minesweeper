#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "CellGrid.hpp"

class PlayingHeader : public sf::Drawable
{
  public:
    explicit PlayingHeader(State::Context const &ctx);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    static constexpr float HEIGHT = 2 * CellGrid::CELL_SIZE;

    sf::RectangleShape headerRect;
};
