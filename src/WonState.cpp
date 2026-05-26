#include "WonState.hpp"

#include <optional>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "LeaderboardEntry.hpp"
#include "LeaderboardManager.hpp"
#include "MenuState.hpp"
#include "PlayingState.hpp"

WonState::WonState(GameStateCtxPtr gameCtx_)
    : GameState(std::move(gameCtx_))
{
    // TODO: setSmiley(WindowLayout::Smiley::Won)
    gameCtx->getHeader().setSmiley(
        gameCtx->getAssets().getTexture("smiley-won"));
    gameCtx->getHeader().setRemainingMines(0);

    LeaderboardEntry entry(gameCtx->getDifficulty(),
                           gameCtx->getHeader().getTime());
    LeaderboardManager::save(entry);
}

void WonState::handleEvent(std::optional<sf::Event> const &event)
{
    if (event->is<sf::Event::MouseButtonPressed>())
    {
        auto const *mouse = event->getIf<sf::Event::MouseButtonPressed>();

        sf::Vector2f headerMousePos =
            gameCtx->getWindow().get().mapPixelToCoords(
                mouse->position, gameCtx->getHeaderView());

        if (mouse->button == sf::Mouse::Button::Left &&
            gameCtx->getHeader().getSmiley().getGlobalBounds().contains(
                headerMousePos))
        {
            restart = true;
            return;
        }
    }
}

void WonState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.setView(gameCtx->getHeaderView());
    target.draw(gameCtx->getHeader(), states);

    target.setView(gameCtx->getGridView());
    target.draw(gameCtx->getGrid(), states);

    sf::RectangleShape highlight{
        {PlayingGrid::CELL_SIZE - PlayingGrid::CELL_PADDING,
         PlayingGrid::CELL_SIZE - PlayingGrid::CELL_PADDING}};

    gameCtx->getGrid().all(
        [&](int col, int row)
        {
            Cell const *cell = gameCtx->getGrid().getCell(col, row);

            if (cell->isMine())
            {
                auto cellPos = sf::Vector2f{sf::Vector2i{col, row}} *
                               PlayingGrid::CELL_SIZE;
                sf::RenderStates cellStates = states;
                cellStates.transform.translate(cellPos);

                if (cell->isFlagged())
                {
                    highlight.setFillColor(sf::Color(0, 190, 0));
                    target.draw(highlight, cellStates);
                }

                target.draw(gameCtx->getGrid().getMineSprite(), cellStates);
            }
        });
}

std::optional<State::Transition> WonState::getTransition()
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (restart)
    {
        auto newGameCtx = std::make_shared<GameStateCtx>(
            ctx, gameCtx->getDifficulty(), gameCtx->getParams());

        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state =
            std::make_unique<PlayingState>(std::move(newGameCtx));
        return transition;
    }

    if (transitionToMenu)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<MenuState>(ctx);
        return transition;
    }

    return std::nullopt;
}

void WonState::print(std::ostream &os) const
{
    os << "WonState";
}
