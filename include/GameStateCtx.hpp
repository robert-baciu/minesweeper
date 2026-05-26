#pragma once

#include "DifficultyParams.hpp"
#include "PlayingHeader.hpp"
#include "StateCtx.hpp"

class GameStateCtx : public StateCtx
{
  public:
    GameStateCtx(StateCtxPtr const &ctx, DifficultyParams params_);

    DifficultyParams const &getParams() const;

    sf::Vector2f getGridSize() const;
    sf::Vector2f getHeaderSize() const;
    sf::Vector2f getWindowSize() const;

    sf::Vector2f getHeaderPos() const;
    sf::Vector2f getGridPos() const;

    sf::View &getHeaderView();
    sf::View const &getHeaderView() const;
    sf::View &getGridView();
    sf::View const &getGridView() const;

    PlayingHeader &getHeader();
    PlayingHeader const &getHeader() const;

    PlayingGrid &getGrid();
    PlayingGrid const &getGrid() const;

    unsigned int getCellCount() const;
    unsigned int getMineCount() const;

    sf::Sprite const &getFlagSprite() const;
    sf::Sprite const &getMineSprite() const;

  private:
    static constexpr float HEADER_H = 2 * PlayingGrid::CELL_SIZE;
    static constexpr auto PADDING = sf::Vector2f(0.5f * PlayingGrid::CELL_SIZE,
                                                 0.5f * PlayingGrid::CELL_SIZE);

    DifficultyParams params;

    sf::Vector2f gridSize;
    sf::Vector2f headerSize;
    sf::Vector2f windowSize;

    sf::Vector2f headerPos;
    sf::Vector2f gridPos;

    sf::View headerView;
    sf::View gridView;

    PlayingHeader header;
    PlayingGrid grid;

    unsigned int cellCount;
    unsigned int mineCount;
};

using GameStateCtxPtr = std::shared_ptr<GameStateCtx>;
