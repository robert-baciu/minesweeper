#include <cstdlib>
#include <random>

#include "Game.hpp"

int main()
{
    std::random_device rd;
    srand(rd());

    Game game;
    while (game.isRunning())
    {
        game.loop();
        // std::cout << game << std::endl;
    }
    return 0;
}
