#pragma once

#include <iostream>
#include <ostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>

#include "GameState.hpp"
#include "Grid.hpp"
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

    static constexpr unsigned int COLS = 32;
    static constexpr unsigned int ROWS = 24;
    static constexpr float CELL_SIZE = 50.0f;

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

    grid.placeRandMines({0, 0}, (COLS * ROWS) / 32);
}

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Transform originalTransform = states.transform;
    sf::RectangleShape cellShape{{CELL_SIZE - 2.0f, CELL_SIZE - 2.0f}};

    for (unsigned int row = 0; row < ROWS; row++)
    {
        for (unsigned int col = 0; col < COLS; col++)
        {
            Cell const &cell = grid.getCell(col, row);

            if (cell.getState() == Cell::State::Unrevealed)
            {
                states.transform = originalTransform;
                states.transform.translate(
                    sf::Vector2f{static_cast<float>(col),
                                 static_cast<float>(row)} *
                    CELL_SIZE);

                if (cell.getType() == Cell::Type::Empty)
                {
                    cellShape.setFillColor(CELL_COLOR);
                }
                else
                {
                    cellShape.setFillColor(sf::Color::Red);
                }

                target.draw(cellShape, states);
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
            auto col = static_cast<unsigned int>(gridPos.x / CELL_SIZE);
            auto row = static_cast<unsigned int>(gridPos.y / CELL_SIZE);

            if (col < COLS && row < ROWS)
            {
                if (mouse->button == sf::Mouse::Button::Left)
                {
                    std::cout << "reveal@[" << col << "," << row << "]"
                              << std::endl;
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
