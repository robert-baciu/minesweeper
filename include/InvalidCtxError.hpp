#pragma once

#include <string>

#include "MinesweeperError.hpp"

class InvalidStateCtxError : public MinesweeperError
{
  public:
    explicit InvalidStateCtxError(std::string const &message)
        : MinesweeperError("InvalidStateCtxError: " + message)
    {
    }
};
