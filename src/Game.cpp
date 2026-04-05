#include "Game.hpp"

#include "ExitingState.hpp"
#include "MenuState.hpp"

Game::Game() : stateContext{assets, window}
{
    assets.load();
    currentState = std::make_unique<MenuState>(stateContext);
    prevUpdateTime = std::chrono::high_resolution_clock::now();
}

bool Game::isLooping() const
{
    return lifecycle != Lifecycle::Stopped;
}

void Game::loop()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime =
        std::chrono::duration<double>(currentTime - prevUpdateTime);
    double dt = elapsedTime.count();
    prevUpdateTime = currentTime;

    handleEvents();
    currentState->update(dt);
    window.get().clear();
    window.get().draw(*currentState);
    window.get().display();

    // TODO: rethink this bad logic
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
