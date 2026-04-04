#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <SFML/Window/Keyboard.hpp>

#include "AssetManager.hpp"
#include "ExitingState.hpp"
#include "GameState.hpp"
#include "GameWindow.hpp"
#include "MenuState.hpp"

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

Game::Game() : stateContext{assets, window}
{
    assets.init();
    currentState = std::make_unique<MenuState>(stateContext);
    lastTime = std::chrono::high_resolution_clock::now();
}

bool Game::isRunning() const
{
    return lifecycle != Lifecycle::Stopped;
}

void Game::loop()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration<double>(currentTime - lastTime);
    double dt = elapsedTime.count();
    lastTime = currentTime;

    handleEvents();
    currentState->update(dt);
    window.get().clear(sf::Color::Black);
    window.get().draw(*currentState);
    window.get().display();

    if (lifecycle == Lifecycle::Exiting && currentState->isReadyToExit())
    {
        lifecycle = Lifecycle::Stopped;
        return;
    }

    if (lifecycle == Lifecycle::ExitRequested)
    {
        if (auto nextState = currentState->getExitState())
        {
            currentState = std::move(nextState);
        }
        else
        {
            currentState = std::make_unique<ExitingState>(stateContext);
        }
        lifecycle = Lifecycle::Exiting;
        return;
    }

    if (lifecycle == Lifecycle::Running)
    {
        if (auto nextState = currentState->getNextState())
        {
            currentState = std::move(nextState);
        }
    }
}

std::ostream &operator<<(std::ostream &os, Game const &game)
{
    os << "Game[window=" << game.window << ", lifecycle=";

    switch (game.lifecycle)
    {
    case Game::Lifecycle::Running:
        os << "Running";
        break;
    case Game::Lifecycle::ExitRequested:
        os << "ExitRequested";
        break;
    case Game::Lifecycle::Exiting:
        os << "Exiting";
        break;
    case Game::Lifecycle::Stopped:
        os << "Stopped";
        break;
    }

    os << ", state=" << *game.currentState << "]";
    return os;
}

void Game::handleEvents()
{
    while (std::optional const event = window.get().pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            if (lifecycle != Lifecycle::Exiting)
            {
                lifecycle = Lifecycle::ExitRequested;
            }
            continue;
        }

        if (event->is<sf::Event::KeyPressed>())
        {
            auto const *key = event->getIf<sf::Event::KeyPressed>();
            if (key->control && key->scancode == sf::Keyboard::Scancode::C)
            {
                if (lifecycle != Lifecycle::Exiting)
                {
                    lifecycle = Lifecycle::ExitRequested;
                }
                continue;
            }
        }

        currentState->handleEvent(event);
    }
}
