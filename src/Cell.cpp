#include "Cell.hpp"

// TODO: Custom exceptions
unsigned int Cell::getAdjacentMines() const
{
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
    this->adjacentMines = adjacentMines;
}

void Cell::setMine()
{
    mine = true;
}

void Cell::setRevealed()
{
    revealed = true;
    flagged = false;
}

void Cell::setFlagged(bool flagged)
{
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
