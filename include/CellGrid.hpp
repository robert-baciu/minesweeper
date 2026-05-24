#pragma once

#include <functional>
#include <optional>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Cell.hpp"
#include "State.hpp"

class CellGrid : public sf::Drawable
{
  public:
    CellGrid(int cols, int rows, State::Context const &ctx);

    [[nodiscard]] int getCols() const;
    [[nodiscard]] int getRows() const;

    [[nodiscard]] Cell *getCell(int col, int row);
    [[nodiscard]] Cell const *getCell(int col, int row) const;
    [[nodiscard]] std::vector<Cell> const &getCells() const;

    void all(std::function<void(int col, int row)> const &callback) const;

    void neighbors(int col, int row,
                   std::function<void(int neighborCol, int neighborRow)> const
                       &callback) const;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    friend std::ostream &operator<<(std::ostream &os, CellGrid const &grid);

    static constexpr float CELL_SIZE = 64.0f;
    static constexpr float CELL_PADDING = 2.0f;

  private:
    [[nodiscard]] bool isInBounds(int col, int row) const;

    static constexpr auto CELL_DIGIT_SIZE =
        static_cast<unsigned int>(CELL_SIZE * 0.9f);

    static constexpr sf::Color CELL_COLOR_UNREVEALED = {127, 127, 127};
    static constexpr sf::Color CELL_COLOR_REVEALED = {190, 190, 190};

    static sf::Color constexpr CELL_DIGIT_COLORS[9] = {
        {0, 0, 0},   {0, 0, 127},   {0, 127, 0},   {190, 0, 0}, {0, 0, 63},
        {127, 0, 0}, {0, 127, 127}, {127, 0, 127}, {63, 63, 63}};

    int const cols;
    int const rows;
    std::vector<Cell> cells;

    mutable sf::RectangleShape cellShape;
    mutable sf::Text cellText;

    sf::Sprite flagSprite;
};
