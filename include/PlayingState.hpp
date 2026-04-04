#pragma once

#include <iostream>
#include <ostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include <string>

#include "GameState.hpp"
#include "Grid.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Transform.hpp"
#include "SFML/Window/Mouse.hpp"

class PlayingState : public GameState
{
  public:
    explicit PlayingState(StateContext const &ctx);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    std::unique_ptr<GameState> getNextState() override;
    void handleEvent(std::optional<sf::Event> const &event) override;
    void print(std::ostream &os) const override;
    void update(double dt) override;

  private:
    static constexpr auto HEADER_HEIGHT = 200.0f;
    static constexpr auto GRID_OFFSET = sf::Vector2f{0.0f, HEADER_HEIGHT};

    static constexpr int COLS = 16;
    static constexpr int ROWS = 24;
    static constexpr float CELL_SIZE = 48.0f;

    static constexpr auto CELL_COLOR = sf::Color{190, 190, 190};
    static constexpr auto CELL_COLOR_HOVER = sf::Color{205, 205, 205};

    Grid grid;
};

PlayingState::PlayingState(StateContext const &ctx)
    : GameState(ctx), grid(COLS, ROWS)
{
    float windowWidth = COLS * CELL_SIZE;
    float windowHeight = ROWS * CELL_SIZE;
    sf::Vector2f windowSize = {windowWidth, windowHeight};
    sf::View view{sf::FloatRect{{0.0f, 0.0f}, {windowWidth, windowHeight}}};

    ctx.window.get().setSize(sf::Vector2u{windowSize});
    ctx.window.get().setView(view);

    grid.placeRandMines({0, 0}, (COLS * ROWS) / 2);
}

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Transform originalTransform = states.transform;
    sf::RectangleShape cellShape{{CELL_SIZE - 2.0f, CELL_SIZE - 2.0f}};

    sf::Text cellText{ctx.assets.getFont("main"), "",
                      static_cast<unsigned int>(CELL_SIZE * 0.9f)};

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            Cell const *cell = grid.getCell(col, row);

            if (cell->getState() == Cell::State::Unrevealed)
            {
                states.transform = originalTransform;
                states.transform.translate(
                    sf::Vector2f{static_cast<float>(col),
                                 static_cast<float>(row)} *
                    CELL_SIZE);

                if (cell->getType() == Cell::Type::Empty)
                {
                    cellShape.setFillColor(CELL_COLOR);
                }

                target.draw(cellShape, states);

                if (cell->getType() == Cell::Type::Empty)
                {
                    if (auto mineCount = cell->getMineCount(); mineCount > 0)
                    {
                        cellText.setString(std::to_string(mineCount));
                        cellText.setOrigin(
                            cellText.getLocalBounds().getCenter());
                        cellText.setPosition(
                            sf::Vector2f{CELL_SIZE / 2.0f, CELL_SIZE / 2.0f});

                        sf::Color cellTextColor;
                        switch (mineCount)
                        {
                        case 1:
                            cellTextColor = sf::Color{0, 0, 127};
                            break;
                        case 2:
                            cellTextColor = sf::Color{0, 127, 0};
                            break;
                        case 3:
                            cellTextColor = sf::Color{127, 0, 0};
                            break;
                        case 4:
                            cellTextColor = sf::Color{0, 0, 63};
                            break;
                        case 5:
                            cellTextColor = sf::Color{63, 0, 0};
                            break;
                        case 6:
                            cellTextColor = sf::Color{0, 127, 127};
                            break;
                        case 7:
                            cellTextColor = sf::Color{127, 0, 127};
                            break;
                        case 8:
                            cellTextColor = sf::Color{63, 63, 63};
                            break;
                        default:
                            throw std::runtime_error("Invalid mine count");
                            break;
                        }

                        cellText.setFillColor(cellTextColor);

                        target.draw(cellText, states);
                    }
                }
            }
            else
            {
                throw std::runtime_error(
                    "This type of Cell is not implemented");
            }
        }
    }
}

void PlayingState::handleEvent(std::optional<sf::Event> const &event)
{
    if (event->is<sf::Event::MouseButtonPressed>())
    {
        auto const *mouse = event->getIf<sf::Event::MouseButtonPressed>();
        sf::Vector2f gridPos =
            ctx.window.get().mapPixelToCoords(mouse->position);

        if (gridPos.x >= 0 && gridPos.y >= 0)
        {
            auto col = static_cast<int>(gridPos.x / CELL_SIZE);
            auto row = static_cast<int>(gridPos.y / CELL_SIZE);

            if (col < COLS && row < ROWS)
            {
                if (mouse->button == sf::Mouse::Button::Left)
                {
                    std::cout << "reveal[" << col << "," << row << "]";
                    Cell *cell = grid.getCell(col, row);
                    if (cell->getType() == Cell::Type::Empty)
                    {
                        std::cout << " " << cell->getMineCount();
                    }
                    std::cout << std::endl;
                }
                else if (mouse->button == sf::Mouse::Button::Right)
                {
                    std::cout << "flag@[" << col << "," << row << "]"
                              << std::endl;
                }
            }
        }
    }
}

std::unique_ptr<GameState> PlayingState::getNextState()
{
    return nullptr;
}

void PlayingState::print(std::ostream &os) const
{
    os << "PlayingState";
}

void PlayingState::update([[maybe_unused]] double dt)
{
}
