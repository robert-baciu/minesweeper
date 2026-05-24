#pragma once

#include <SFML/Window/Window.hpp>

#include "CellGrid.hpp"
#include "PlayingHeader.hpp"

struct WindowLayout
{
    PlayingHeader header;
    sf::View headerView;
    CellGrid grid;
    sf::View gridView;
};
