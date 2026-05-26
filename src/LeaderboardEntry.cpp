#include "LeaderboardEntry.hpp"

#include <utility>

LeaderboardEntry::LeaderboardEntry(std::string difficulty_, float time_)
    : difficulty(std::move(difficulty_)),
      time(time_)
{
}

std::string LeaderboardEntry::getDifficulty() const
{
    return difficulty;
}

float LeaderboardEntry::getTime() const
{
    return time;
}
