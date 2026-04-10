#include "GameWindow.hpp"

#include "GameConfig.hpp"
#include "SFML/System/Vector2.hpp"

GameWindow::GameWindow()
{
    auto desktop = sf::VideoMode::getDesktopMode();
    auto desktopHeight = static_cast<float>(desktop.size.y);

    float windowRatio = 4.0f / 5.0f;

    float windowWidth = desktopHeight * 0.4f;

    if (windowWidth < 400.0f)
    {
        windowWidth = 400.0f;
    }

    if (windowWidth > 800.0f)
    {
        windowWidth = 800.0f;
    }

    float windowHeight = windowWidth / windowRatio;
    sf::Vector2u windowSize = {static_cast<unsigned int>(windowWidth),
                               static_cast<unsigned int>(windowHeight)};

    defaultSize = windowSize;

    window.create(sf::VideoMode(defaultSize), GameConfig::TITLE,
                  sf::Style::Default);
    window.setVerticalSyncEnabled(true);
}

sf::RenderWindow &GameWindow::get()
{
    return window;
}

sf::Vector2u GameWindow::getDefaultSize()
{
    return defaultSize;
}

std::ostream &operator<<(std::ostream &os, GameWindow const &window)
{
    auto width = window.window.getSize().x;
    auto height = window.window.getSize().y;
    os << "GameWindow[width=" << width << ", height=" << height << "]";
    return os;
}
