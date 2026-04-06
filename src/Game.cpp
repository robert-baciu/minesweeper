#include "Game.hpp"

#include "MenuState.hpp"

Game::Game() : stateContext{assets, window}
{
    assets.load();
    currentState = std::make_unique<MenuState>(stateContext);
    prevUpdateTime = std::chrono::high_resolution_clock::now();
}

bool Game::isRunning() const
{
    return running;
}

void Game::run()
{
    if (auto transition = currentState->getTransition())
    {
        if (transition->action == State::Action::Change)
        {
            currentState = std::move(transition->state);
        }
        else if (transition->action == State::Action::Exit)
        {
            running = false;
            return;
        }
    }

    auto currentTime = std::chrono::high_resolution_clock::now();
    double dt =
        std::chrono::duration<double>(currentTime - prevUpdateTime).count();
    prevUpdateTime = currentTime;

    handleEvents();

    currentState->update(dt);

    window.get().clear();
    window.get().draw(*currentState);
    window.get().display();
}

void Game::handleEvents()
{
    while (std::optional const event = window.get().pollEvent())
    {
        bool quit = false;

        if (event->is<sf::Event::Closed>())
        {
            quit = true;
        }

        if (event->is<sf::Event::KeyPressed>())
        {
            auto const *key = event->getIf<sf::Event::KeyPressed>();
            if (key->control && key->scancode == sf::Keyboard::Scancode::C)
            {
                quit = true;
            }
        }

        if (quit)
        {
            if (!requestedExit)
            {
                currentState->requestExit();
                requestedExit = true;
            }
            continue;
        }

        currentState->handleEvent(event);
    }
}

std::ostream &operator<<(std::ostream &os, Game const &game)
{
    os << "Game[running=" << (game.running ? "yes" : "no")
       << ", exitRequested=" << (game.requestedExit ? "yes" : "no")
       << ", state=" << *game.currentState
       << ", stateContext=" << game.stateContext << "]";
    return os;
}
