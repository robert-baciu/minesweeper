#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

class GameWindow
{
  public:
    GameWindow();

    sf::RenderWindow &get();

    friend std::ostream &operator<<(std::ostream &os,
                                    GameWindow const &gameWindow);

  private:
    sf::RenderWindow window;
};
