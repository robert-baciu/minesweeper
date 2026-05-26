#include "LeaderboardEntry.hpp"

#include "Difficulty.hpp"

LeaderboardEntry::LeaderboardEntry(Difficulty difficulty, float time)
    : difficulty(difficulty),
      time(time)
{
}

// cppcheck-suppress unusedFunction
Difficulty LeaderboardEntry::getDifficulty() const
{
    return difficulty;
}

// cppcheck-suppress unusedFunction
float LeaderboardEntry::getTime() const
{
    return time;
}
