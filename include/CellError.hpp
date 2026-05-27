#pragma once

#include <stdexcept>

class CellError : public std::runtime_error
{
  public:
    ~CellError() override = default;

  protected:
    explicit CellError(std::string const &message)
        : std::runtime_error("CellError: " + message)
    {
    }
};

class MineCellError : public CellError
{
  public:
    explicit MineCellError(std::string const &func)
        : CellError(func + " called on mine cell")
    {
    }
};

class RevealedCellError : public CellError
{
  public:
    explicit RevealedCellError(std::string const &func)
        : CellError(func + " called on revealed cell")
    {
    }
};
