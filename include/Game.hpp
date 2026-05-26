#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <SFML/Window/Keyboard.hpp>

#include "AssetManager.hpp"
#include "State.hpp"
#include "Window.hpp"

class Game
{
  public:
    Game();

    bool isRunning() const;
    void run();

    friend std::ostream &operator<<(std::ostream &os, Game const &game);

  private:
    void handleEvents();

    bool running = true;
    bool requestedExit = false;

    AssetManager assets;
    Window window;

    std::vector<std::unique_ptr<State>> states;

    std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
};
