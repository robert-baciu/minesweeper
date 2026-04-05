#include "ExitingState.hpp"

ExitingState::ExitingState(StateContext const &ctx) : GameState(ctx)
{
}

void ExitingState::draw([[maybe_unused]] sf::RenderTarget &target,
                        [[maybe_unused]] sf::RenderStates states) const
{
}

void ExitingState::update([[maybe_unused]] double dt)
{
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
