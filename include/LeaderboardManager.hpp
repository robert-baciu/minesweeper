#pragma once

#include <vector>

#include "LeaderboardEntry.hpp"

class LeaderboardManager
{
  public:
    static std::vector<LeaderboardEntry> loadAll();

    static void save(LeaderboardEntry const &entry);
    static void save(std::vector<LeaderboardEntry> const &entries);

  private:
    static inline std::string const LEADERBOARD_FILE = "leaderboard.csv";
};
