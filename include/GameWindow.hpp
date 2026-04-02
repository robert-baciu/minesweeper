#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

class GameWindow
{
  public:
    GameWindow(unsigned int width, unsigned int height);
    ~GameWindow();
    sf::RenderWindow &get();
    bool isClosed() const;

  private:
    static inline std::string const title = "Minesweeper";
    sf::RenderWindow window;
    bool closed;
};

GameWindow::GameWindow(unsigned int width, unsigned int height)
{
    // NOTE: Sync window title with env variable APP_WINDOW from
    // .github/workflows/cmake.yml
    window.create(sf::VideoMode({width, height}), title, sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    closed = false;
}

GameWindow::~GameWindow()
{
    window.close();
}

sf::RenderWindow &GameWindow::get()
{
    return window;
}

bool GameWindow::isClosed() const
{
    return closed;
}
