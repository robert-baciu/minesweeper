#include "Window.hpp"

#include <SFML/System/Vector2.hpp>

#include "GameConfig.hpp"

Window::Window()
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

    gui.setWindow(window);

    auto font = tgui::Font("assets/fonts/VCR_OSD_MONO_1.001.ttf");
    gui.setFont(font);
    gui.setTextSize(32);

    auto theme = tgui::Theme::create("assets/themes/Dark.txt");
    tgui::Theme::setDefault(theme);
}

sf::RenderWindow &Window::get()
{
    return window;
}

sf::Vector2u Window::getDefaultSize()
{
    return defaultSize;
}

tgui::Gui &Window::getGui()
{
    return gui;
}

std::ostream &operator<<(std::ostream &os, Window const &window)
{
    auto width = window.window.getSize().x;
    auto height = window.window.getSize().y;
    os << "GameWindow[width=" << width << ", height=" << height << "]";
    return os;
}
