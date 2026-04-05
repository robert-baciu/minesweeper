#pragma once

#include <string>

class GameConfig
{
  public:
    // NOTE: Sync window title with env variable APP_WINDOW from
    // .github/workflows/cmake.yml
    static inline std::string const TITLE = "Minesweeper";
};
