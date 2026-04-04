#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "AssetManager.hpp"
#include "GameWindow.hpp"

struct StateContext
{
    AssetManager const &assets;
    GameWindow &window;

    explicit StateContext(AssetManager const &assets, GameWindow &window)
        : assets(assets), window(window)
    {
    }
};
