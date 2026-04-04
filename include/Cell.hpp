#pragma once

#include <ostream>

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
    Type getType() const;
    void setType(Type type);

    friend std::ostream &operator<<(std::ostream &os, Cell const &cell);

  private:
    State state = State::Unrevealed;
    Type type = Type::Empty;
};

Cell::State Cell::getState() const
{
    return state;
}

Cell::Type Cell::getType() const
{
    return type;
}

void Cell::setType(Cell::Type newType)
{
    type = newType;
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
