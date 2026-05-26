#pragma once

#include <string>

class LeaderboardEntry
{
  public:
    // TODO: String or enum?
    LeaderboardEntry(std::string difficulty_, float time_);

    [[nodiscard]] std::string getDifficulty() const;

    [[nodiscard]] float getTime() const;

  private:
    std::string difficulty;
    float time;
};
