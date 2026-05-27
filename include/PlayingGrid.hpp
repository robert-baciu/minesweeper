#pragma once

#include <functional>
#include <optional>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

#include "Cell.hpp"
#include "DifficultyParams.hpp"
#include "StateCtx.hpp"

class PlayingGrid : public sf::Drawable
{
  public:
    PlayingGrid(StateCtx const &ctx, DifficultyParams params,
                sf::Vector2f position);

    void handleEvent(std::optional<sf::Event> const &event);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void all(std::function<void(int col, int row)> const &callback) const;

    void neighbors(int col, int row,
                   std::function<void(int neighborCol, int neighborRow)> const
                       &callback) const;

    [[nodiscard]] int getCols() const;
    [[nodiscard]] int getRows() const;

    [[nodiscard]] Cell *getCell(int col, int row);
    [[nodiscard]] Cell const *getCell(int col, int row) const;

    [[nodiscard]] std::vector<Cell> const &getCells() const;

    sf::Vector2f getSize() const;
    sf::Vector2f getPosition() const;

    enum class GridState
    {
        Ongoing,
        Lost,
        Won
    };

    GridState getState() const;
    sf::Vector2i getDetonatedPos() const;

    sf::Sprite getFlagSprite() const;
    sf::Sprite getMineSprite() const;

    static sf::RectangleShape getCellRect();

    unsigned int getMineCount() const;
    unsigned int getFlagCount() const;

    bool isStarted() const;

    friend std::ostream &operator<<(std::ostream &os, PlayingGrid const &grid);

    static constexpr float CELL_SIZE = 64.0f;
    static constexpr float CELL_PADDING = 2.0f;

  private:
    void leftClick(int col, int row);
    void rightClick(int col, int row);

    void firstReveal(int col, int row);
    void floodReveal(int col, int row);
    void chordReveal(int col, int row);

    [[nodiscard]] bool isInBounds(int col, int row) const;

    static constexpr auto CELL_DIGIT_SIZE =
        static_cast<unsigned int>(CELL_SIZE * 0.9f);

    static constexpr sf::Color CELL_COLOR_UNREVEALED{127, 127, 127};
    static constexpr sf::Color CELL_COLOR_REVEALED{190, 190, 190};
    static constexpr sf::Color CELL_DIGIT_COLORS[9] = {
        {0, 0, 0},   {0, 0, 127},   {0, 127, 0},   {190, 0, 0}, {0, 0, 63},
        {127, 0, 0}, {0, 127, 127}, {127, 0, 127}, {63, 63, 63}};

    StateCtx const &ctx;

    int const cols;
    int const rows;

    sf::Vector2f size;
    sf::Vector2f position;

    std::vector<Cell> cells;

    GridState gridState;
    sf::Vector2i detonatedPos;

    mutable sf::Text cellText;

    sf::Sprite const flagSprite;
    sf::Sprite const mineSprite;

    unsigned int mineCount;
    unsigned int revealedCount;
    unsigned int flagCount;

    bool started;
};
