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

    bool isLooping() const;
    void loop();

    friend std::ostream &operator<<(std::ostream &os, Game const &game);

  private:
    enum class Lifecycle
    {
        Running,
        ExitRequested,
        Exiting,
        Stopped
    };

    void handleEvents();

    AssetManager assets;
    GameWindow window;
    StateContext const stateContext;

    Lifecycle lifecycle = Lifecycle::Running;

    std::unique_ptr<GameState> currentState;

    std::chrono::time_point<std::chrono::high_resolution_clock> prevUpdateTime;
};
