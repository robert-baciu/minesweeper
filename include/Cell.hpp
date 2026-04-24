#pragma once

#include <ostream>

class Cell
{
  public:
    enum class State
    {
        Hidden,
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
    State state = State::Hidden;
    Type type = Type::Empty;

    unsigned int mineCount = 0;
};
