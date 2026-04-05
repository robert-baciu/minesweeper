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
