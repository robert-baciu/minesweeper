#include "Game.hpp"

int main()
{
    Game game;
    while (game.isRunning())
    {
        game.run();
        std::cout << game << std::endl;
    }
    return 0;
}
