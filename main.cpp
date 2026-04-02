#include "Game.hpp"

int main()
{
    Game game;
    while (game.isRunning())
    {
        game.loop();
        std::cout << game << std::endl;
    }
    return 0;
}
