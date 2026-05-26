#pragma once

#include <stdexcept>

class InvalidDifficultyError : public std::runtime_error
{
  public:
    InvalidDifficultyError()
        : std::runtime_error("Invalid difficulty")
    {
    }
};
