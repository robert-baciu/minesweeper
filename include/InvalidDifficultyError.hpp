#pragma once

#include <stdexcept>

class InvalidDifficultyError : public std::runtime_error
{
  public:
    explicit InvalidDifficultyError(std::string const &diff)
        : std::runtime_error("Invalid difficulty: \"" + diff + "\"")
    {
    }
};
