#include "Game.hpp"

int main()
{
    Game game;
    while (game.isLooping())
    {
        game.loop();
        // std::cout << game << std::endl;
    }
    return 0;
}
