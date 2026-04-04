#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "GameConfig.hpp"

class GameWindow
{
  public:
    GameWindow();
    sf::RenderWindow &get();

    friend std::ostream &operator<<(std::ostream &os,
                                    GameWindow const &gameWindow);

  private:
    sf::RenderWindow window;
    sf::View view;
};

GameWindow::GameWindow()
{
    auto desktop = sf::VideoMode::getDesktopMode();
    auto desktopHeight = static_cast<float>(desktop.size.y);

    float windowRatio = 4.0f / 5.0f;
    float windowWidth = desktopHeight * 0.4f;
    float windowHeight = windowWidth / windowRatio;
    sf::Vector2u windowSize = {static_cast<unsigned int>(windowWidth),
                               static_cast<unsigned int>(windowHeight)};

    window.create(sf::VideoMode(windowSize), GameConfig::TITLE,
                  sf::Style::Default);
    window.setVerticalSyncEnabled(true);
}

sf::RenderWindow &GameWindow::get()
{
    return window;
}

std::ostream &operator<<(std::ostream &os, GameWindow const &window)
{
    auto width = window.window.getSize().x;
    auto height = window.window.getSize().y;
    os << "GameWindow[width=" << width << ", height=" << height << "]";
    return os;
}
