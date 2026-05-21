#include "Cell.hpp"

#include "CellError.hpp"

unsigned int Cell::getAdjacentMines() const
{
    if (mine == true)
    {
        throw MineCellError("Cell::getAdjacentMines()");
    }

    return adjacentMines;
}

bool Cell::isMine() const
{
    return mine;
}

bool Cell::isRevealed() const
{
    return revealed;
}

bool Cell::isFlagged() const
{
    return flagged;
}

void Cell::setAdjacentMines(unsigned int adjacentMines)
{
    if (mine == true)
    {
        throw MineCellError("Cell::setAdjacentMines()");
    }

    this->adjacentMines = adjacentMines;
}

void Cell::setMine()
{
    if (revealed == true)
    {
        throw RevealedCellError("Cell::setMine()");
    }

    mine = true;
    flagged = false;
}

void Cell::setRevealed()
{
    if (mine == true)
    {
        throw MineCellError("Cell::setRevealed()");
    }

    revealed = true;
    flagged = false;
}

void Cell::setFlagged(bool flagged)
{
    if (revealed == true)
    {
        throw RevealedCellError("Cell::setFlagged()");
    }

    this->flagged = flagged;
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
