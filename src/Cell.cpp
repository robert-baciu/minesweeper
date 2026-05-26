#include "Cell.hpp"

#include "CellError.hpp"

// cppcheck-suppress unusedFunction
unsigned int Cell::getAdjacentMines() const
{
    if (mine == true)
    {
        throw MineCellError("Cell::getAdjacentMines()");
    }

    return adjacentMines;
}

// cppcheck-suppress unusedFunction
bool Cell::isMine() const
{
    return mine;
}

// cppcheck-suppress unusedFunction
bool Cell::isRevealed() const
{
    if (mine == true)
    {
        throw MineCellError("Cell::isRevealed()");
    }
    return revealed;
}

// cppcheck-suppress unusedFunction
bool Cell::isFlagged() const
{
    return flagged;
}

// cppcheck-suppress unusedFunction
void Cell::setAdjacentMines(unsigned int adjacentMines_)
{
    if (mine == true)
    {
        throw MineCellError("Cell::setAdjacentMines()");
    }

    adjacentMines = adjacentMines_;
}

// cppcheck-suppress unusedFunction
void Cell::setMine()
{
    if (revealed == true)
    {
        throw RevealedCellError("Cell::setMine()");
    }

    mine = true;
    flagged = false;
}

// cppcheck-suppress unusedFunction
void Cell::setRevealed()
{
    if (mine == true)
    {
        throw MineCellError("Cell::setRevealed()");
    }

    revealed = true;
    flagged = false;
}

// cppcheck-suppress unusedFunction
void Cell::setFlagged(bool flagged_)
{
    if (revealed == true)
    {
        throw RevealedCellError("Cell::setFlagged()");
    }

    flagged = flagged_;
}

std::ostream &operator<<(std::ostream &os, Cell const &cell)
{
    os << "Cell[";
    if (cell.mine == true)
    {
        os << "Mine";
    }
    else if (cell.revealed == false)
    {
        if (cell.flagged == false)
        {
            os << "Unrevealed";
        }
        else
        {
            os << "Flagged";
        }
    }
    else
    {
        os << "Revealed";
    }
    os << "]";
    return os;
}
