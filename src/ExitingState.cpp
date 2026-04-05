#include "ExitingState.hpp"

ExitingState::ExitingState(StateContext const &ctx) : GameState(ctx)
{
}

void ExitingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape bg;
    bg.setSize(target.getView().getSize());
    bg.setFillColor(sf::Color::White);
    target.draw(bg, states);
}

std::unique_ptr<GameState> ExitingState::getNextState()
{
    return nullptr;
}

bool ExitingState::isReadyToExit()
{
    return readyToExit;
}

void ExitingState::print(std::ostream &os) const
{
    os << "ExitingState[readyToExit=" << readyToExit << "]";
}

void ExitingState::update([[maybe_unused]] double dt)
{
}
