#pragma once

#include "MinesweeperError.hpp"

class InvalidDifficultyError : public MinesweeperError
{
  public:
    InvalidDifficultyError()
        : MinesweeperError("InvalidDifficultyError: Invalid difficulty")
    {
    }
};
