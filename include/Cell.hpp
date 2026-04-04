#pragma once

#include <ostream>
#include <stdexcept>

class Cell
{
  public:
    enum class State
    {
        Unrevealed,
        Revealed,
        Flagged
    };

    enum class Type
    {
        Empty,
        Mine
    };

    State getState() const;
    void setState(State newState);
    Type getType() const;
    void setType(Type type);
    unsigned int getMineCount() const;
    void setMineCount(unsigned int newMineCount);

    friend std::ostream &operator<<(std::ostream &os, Cell const &cell);

  private:
    unsigned int mineCount = 0;
    State state = State::Unrevealed;
    Type type = Type::Empty;
};

Cell::State Cell::getState() const
{
    return state;
}

void Cell::setState(State newState)
{
    state = newState;
}

Cell::Type Cell::getType() const
{
    return type;
}

void Cell::setType(Cell::Type newType)
{
    type = newType;
}

unsigned int Cell::getMineCount() const
{
    if (type != Type::Empty)
    {
        throw std::runtime_error("getMineCount() called on a non-empty cell");
    }
    return mineCount;
}

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
    os << "Cell[";
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
