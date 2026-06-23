#pragma once

#include "MinesweeperError.hpp"

class CellError : public MinesweeperError
{
  public:
    ~CellError() override = default;

  protected:
    explicit CellError(std::string const &message)
        : MinesweeperError("CellError: " + message)
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
