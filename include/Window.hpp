#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

class Window
{
  public:
    Window();

    sf::RenderWindow &get();

    sf::Vector2u getDefaultSize();

    tgui::Gui &getGui();

    friend std::ostream &operator<<(std::ostream &os, Window const &gameWindow);

  private:
    sf::RenderWindow window;

    sf::Vector2u defaultSize;

    tgui::Gui gui;
};
