#pragma once

#include <ostream>

class Cell
{
  public:
    Cell();

    [[nodiscard]] unsigned int getAdjacentMines() const;
    [[nodiscard]] bool isMine() const;
    [[nodiscard]] bool isRevealed() const;
    [[nodiscard]] bool isFlagged() const;

    void setAdjacentMines(unsigned int adjacentMines_);
    void setMine();
    void setRevealed();
    void setFlagged(bool flagged_);

    friend std::ostream &operator<<(std::ostream &os, Cell const &cell);

  private:
    unsigned int adjacentMines;

    bool mine;
    bool revealed;
    bool flagged;
};
