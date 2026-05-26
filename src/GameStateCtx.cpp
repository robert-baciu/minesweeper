#include "GameStateCtx.hpp"

GameStateCtx::GameStateCtx(StateCtxPtr const &ctx, DifficultyParams params_)
    : StateCtx(*ctx),
      params(params_),

      gridSize(static_cast<float>(params.getCols()) * PlayingGrid::CELL_SIZE,
               static_cast<float>(params.getRows()) * PlayingGrid::CELL_SIZE),
      headerSize(gridSize.x, HEADER_H),
      windowSize(PADDING.x + gridSize.x + PADDING.x,
                 PADDING.y + HEADER_H + PADDING.y + gridSize.y + PADDING.y),

      headerPos(PADDING.x, PADDING.y),
      gridPos(PADDING.x, headerPos.y + headerSize.y + PADDING.y),

      header(ctx),
      grid(ctx, params.getCols(), params.getRows()),

      cellCount(params.getCols() * params.getRows()),
      mineCount(static_cast<unsigned int>(static_cast<float>(cellCount) *
                                          params.getMineDensity()))
{
}

DifficultyParams const &GameStateCtx::getParams() const
{
    return params;
}

sf::Vector2f GameStateCtx::getGridSize() const
{
    return gridSize;
}

sf::Vector2f GameStateCtx::getHeaderSize() const
{
    return headerSize;
}

sf::Vector2f GameStateCtx::getWindowSize() const
{
    return windowSize;
}

sf::Vector2f GameStateCtx::getHeaderPos() const
{
    return headerPos;
}

sf::Vector2f GameStateCtx::getGridPos() const
{
    return gridPos;
}

sf::View &GameStateCtx::getHeaderView()
{
    return headerView;
}

sf::View const &GameStateCtx::getHeaderView() const
{
    return headerView;
}

sf::View &GameStateCtx::getGridView()
{
    return gridView;
}

sf::View const &GameStateCtx::getGridView() const
{
    return gridView;
}

unsigned int GameStateCtx::getCellCount() const
{
    return cellCount;
}

unsigned int GameStateCtx::getMineCount() const
{
    return mineCount;
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
