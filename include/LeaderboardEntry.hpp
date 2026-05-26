#pragma once

#include "Difficulty.hpp"

class LeaderboardEntry
{
  public:
    LeaderboardEntry(Difficulty difficulty, float time);

    [[nodiscard]] Difficulty getDifficulty() const;
    [[nodiscard]] float getTime() const;

  private:
    Difficulty difficulty;
    float time;
};
