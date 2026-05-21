#pragma once

#include <ostream>

class Cell
{
  public:
    [[nodiscard]] unsigned int getAdjacentMines() const;
    [[nodiscard]] bool isMine() const;
    [[nodiscard]] bool isRevealed() const;
    [[nodiscard]] bool isFlagged() const;

    void setAdjacentMines(unsigned int adjacentMines);
    void setMine();
    void setRevealed();
    void setFlagged(bool flagged);

    friend std::ostream &operator<<(std::ostream &os, Cell const &cell);

  private:
    unsigned int adjacentMines = 0;
    bool mine = false;
    bool revealed = false;
    bool flagged = false;
};
