#include "Cell.hpp"

// cppcheck-suppress unusedFunction
Cell::State Cell::getState() const
{
    return state;
}

// cppcheck-suppress unusedFunction
void Cell::setState(State newState)
{
    state = newState;
}

// cppcheck-suppress unusedFunction
Cell::Type Cell::getType() const
{
    return type;
}

// cppcheck-suppress unusedFunction
void Cell::setType(Cell::Type newType)
{
    type = newType;
}

// cppcheck-suppress unusedFunction
unsigned int Cell::getMineCount() const
{
    if (type != Type::Empty)
    {
        throw std::runtime_error("getMineCount() called on a non-empty cell");
    }
    return mineCount;
}

// cppcheck-suppress unusedFunction
void Cell::setMineCount(unsigned int newMineCount)
{
    if (type != Type::Empty)
    {
        throw std::runtime_error("setMineCount() called on a non-empty cell");
    }
    mineCount = newMineCount;
}

std::ostream &operator<<(std::ostream &os, Cell const &cell)
{
    os << "Cell[state=";
    switch (cell.state)
    {
    case Cell::State::Unrevealed:
        os << "Unrevealed";
        break;
    case Cell::State::Revealed:
        os << "Revealed";
        break;
    case Cell::State::Flagged:
        os << "Flagged";
        break;
    }

    os << ", type=";
    switch (cell.type)
    {
    case Cell::Type::Empty:
        os << "Empty";
        break;
    case Cell::Type::Mine:
        os << "Mine";
        break;
    }
    os << "]";
    return os;
}
