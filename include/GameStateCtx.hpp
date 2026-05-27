#pragma once

#include "Difficulty.hpp"
#include "DifficultyParams.hpp"
#include "PlayingHeader.hpp"
#include "StateCtx.hpp"

class GameStateCtx : public StateCtx
{
  public:
    GameStateCtx(StateCtx &ctx, Difficulty difficulty, DifficultyParams params);

    Difficulty getDifficulty() const;
    DifficultyParams const &getParams() const;

    PlayingHeader &getHeader();
    PlayingHeader const &getHeader() const;

    PlayingGrid &getGrid();
    PlayingGrid const &getGrid() const;

  private:
    static constexpr float HEADER_H = 2 * PlayingGrid::CELL_SIZE;
    static constexpr sf::Vector2f PADDING{0.5f * PlayingGrid::CELL_SIZE,
                                          0.5f * PlayingGrid::CELL_SIZE};

    Difficulty difficulty;
    DifficultyParams params;

    sf::Vector2f gridPos;
    PlayingGrid grid;

    sf::Vector2f headerSize;
    sf::Vector2f headerPos;
    PlayingHeader header;
};
