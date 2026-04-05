#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <SFML/Window/Keyboard.hpp>

#include "AssetManager.hpp"
#include "GameState.hpp"
#include "GameWindow.hpp"

class Game
{
  public:
    Game();
    bool isRunning() const;
    void loop();

    friend std::ostream &operator<<(std::ostream &os, Game const &game);

  private:
    AssetManager assets;
    GameWindow window;
    StateContext const stateContext;

    enum class Lifecycle
    {
        Running,
        ExitRequested,
        Exiting,
        Stopped
    };
    Lifecycle lifecycle = Lifecycle::Running;
    std::unique_ptr<GameState> currentState;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

    void handleEvents();
};
