#include "Game.hpp"

int main()
{
    try
    {
        Game game;
        while (game.isRunning())
        {
            game.run();
            std::cout << game << std::endl;
        }
    }
    catch (std::exception const &e)
    {
        std::cerr << "[Exception] " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "[Exception] Unknown error" << std::endl;
        return 2;
    }

    return 0;
}
