#pragma once

#include <stdexcept>

class MinesweeperError : public std::runtime_error
{
  public:
    explicit MinesweeperError(std::string const &msg)
        : std::runtime_error(msg)
    {
    }
};
