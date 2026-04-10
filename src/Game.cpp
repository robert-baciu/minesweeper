#include "Game.hpp"

#include "MenuState.hpp"

Game::Game() : stateContext{assets, window}
{
    assets.load();
    states.push_back(std::make_unique<MenuState>(stateContext));
    prevUpdateTime = std::chrono::high_resolution_clock::now();
}

bool Game::isRunning() const
{
    return running;
}

void Game::run()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    double dt =
        std::chrono::duration<double>(currentTime - prevUpdateTime).count();
    prevUpdateTime = currentTime;

    auto const &currentState = states.back();

    if (auto transition = currentState->getTransition())
    {
        if (transition->action == State::Action::Change)
        {
            states.clear();
            states.push_back(std::move(transition->state));
        }
        else if (transition->action == State::Action::Push)
        {
            states.push_back(std::move(transition->state));
        }
        else if (transition->action == State::Action::Pop)
        {
            states.pop_back();
        }
        else if (transition->action == State::Action::Exit)
        {
            running = false;
        }
        return;
    }

    handleEvents();

    currentState->update(dt);

    window.get().clear();
    for (auto &state : states)
    {
        window.get().draw(*state);
    }
    currentState->getGui().draw();
    window.get().display();
}

void Game::handleEvents()
{
    auto const &currentState = states.back();

    while (std::optional const event = window.get().pollEvent())
    {
        currentState->getGui().handleEvent(*event);

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
       << ", exitRequested=" << (game.requestedExit ? "yes" : "no");

    os << ", states={";
    bool firstState = true;
    for (auto const &state : game.states)
    {
        if (!firstState)
        {
            os << ", ";
        }
        os << *state;
        firstState = false;
    }

    os << "}";

    os << ", stateContext=" << game.stateContext << "]";
    return os;
}
