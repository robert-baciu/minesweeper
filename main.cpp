#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window;

    // NOTE: sync with env variable APP_WINDOW from
    // .github/workflows/cmake.yml:31

    window.create(sf::VideoMode({800, 600}), "Minesweeper", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        bool shouldExit = false;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (event->is<sf::Event::KeyPressed>())
            {
                const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    shouldExit = true;
                }
            }
        }

        if (shouldExit)
        {
            window.close();
            break;
        }

        window.clear();
        window.display();
    }

    return 0;
}
