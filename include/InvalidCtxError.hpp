#pragma once

#include <stdexcept>
#include <string>

class InvalidStateCtxError : public std::runtime_error
{
  public:
    explicit InvalidStateCtxError(std::string const &message)
        : std::runtime_error("InvalidStateCtxError: " + message)
    {
    }
};
