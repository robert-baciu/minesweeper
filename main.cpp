#include "Game.hpp"
#include "MinesweeperError.hpp"

int main()
{
    try
    {
        Game game;
        while (game.isRunning())
        {
            game.run();
            // std::cout << game << std::endl;
        }
    }
    catch (MinesweeperError const &e)
    {
        std::cerr << "[Minesweeper error] " << e.what() << std::endl;
        return 1;
    }
    catch (std::exception const &e)
    {
        std::cerr << "[Standard error] " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "[Unknown error] ?" << std::endl;
        return 1;
    }

    return 0;
}
