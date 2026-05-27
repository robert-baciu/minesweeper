#include "GameStateCtx.hpp"

#include <SFML/System/Vector2.hpp>

#include "PlayingGrid.hpp"

GameStateCtx::GameStateCtx(StateCtx &ctx, Difficulty difficulty,
                           DifficultyParams params)
    : StateCtx(ctx.getAssets(), ctx.getWindow()), // TODO: copy constructor
      difficulty(difficulty),
      params(params),

      gridPos(PADDING.x, PADDING.y + HEADER_H + PADDING.y),
      grid(*this, params, gridPos),

      headerSize(grid.getSize().x, HEADER_H),
      headerPos(PADDING.x, PADDING.y),

      header(*this, headerSize, headerPos)
{
    sf::Vector2f windowSize = {PADDING.x + grid.getSize().x + PADDING.x,
                               PADDING.y + headerSize.y + PADDING.y +
                                   grid.getSize().y + PADDING.y};

    ctx.getWindow().get().setSize(sf::Vector2u(windowSize));
    ctx.getWindow().get().setView(
        sf::View(sf::FloatRect(sf::Vector2f(0, 0), windowSize)));
}

Difficulty GameStateCtx::getDifficulty() const
{
    return difficulty;
}

DifficultyParams const &GameStateCtx::getParams() const
{
    return params;
}

PlayingGrid &GameStateCtx::getGrid()
{
    return grid;
}

PlayingGrid const &GameStateCtx::getGrid() const
{
    return grid;
}

PlayingHeader &GameStateCtx::getHeader()
{
    return header;
}

PlayingHeader const &GameStateCtx::getHeader() const
{
    return header;
}
