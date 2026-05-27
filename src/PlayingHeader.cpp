#include "PlayingHeader.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

PlayingHeader::PlayingHeader(StateCtx const &ctx, sf::Vector2f size,
                             sf::Vector2f position)
    : ctx(ctx),
      header(size),
      position(position),
      clockText(ctx.getAssets().getMainFont()),
      remaining(0),
      remainingText(ctx.getAssets().getMainFont()),
      smiley(ctx.getAssets().getTexture("smiley-playing")),
      restart(false)
{
    header.setFillColor(HEADER_COLOR);

    clock.reset();

    clockText.setOrigin(
        sf::Vector2f(0, clockText.getLocalBounds().size.y / 2.0f));
    clockText.setPosition(
        sf::Vector2f(PlayingGrid::CELL_SIZE, header.getSize().y / 2.0f));
    clockText.setFillColor(TEXT_COLOR);
    clockText.setCharacterSize(TEXT_SIZE);

    remainingText.setOrigin(
        sf::Vector2f(remainingText.getLocalBounds().size.x,
                     remainingText.getLocalBounds().size.y / 2.0f));
    remainingText.setPosition(
        sf::Vector2f(header.getSize().x - PlayingGrid::CELL_SIZE,
                     header.getSize().y / 2.0f));
    remainingText.setFillColor(TEXT_COLOR);
    remainingText.setCharacterSize(TEXT_SIZE);

    smiley.setScale(
        sf::Vector2f(SMILEY_SIZE.x / smiley.getLocalBounds().size.x,
                     SMILEY_SIZE.y / smiley.getLocalBounds().size.y));
    smiley.setOrigin(smiley.getLocalBounds().getCenter());
    smiley.setPosition(header.getSize() / 2.0f);
}

void PlayingHeader::handleEvent(std::optional<sf::Event> const &event)
{
    if (auto const *mouse = event->getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f globalPos =
            ctx.getWindow().get().mapPixelToCoords(mouse->position);
        sf::Vector2f localPos = globalPos - position;

        sf::FloatRect bounds(sf::Vector2f(0, 0), header.getSize());

        if (!bounds.contains(localPos))
        {
            return;
        }

        if (smiley.getGlobalBounds().contains(localPos))
        {
            restart = true;
        }
    }
}

void PlayingHeader::update()
{
    std::stringstream clockSeconds;
    clockSeconds << std::fixed << std::setprecision(1)
                 << clock.getElapsedTime().asSeconds();

    clockText.setString(clockSeconds.str());
    clockText.setOrigin(
        sf::Vector2f(0.0f, clockText.getLocalBounds().getCenter().y));

    remainingText.setString(std::to_string(remaining));
    remainingText.setOrigin(
        sf::Vector2f(remainingText.getLocalBounds().size.x,
                     remainingText.getLocalBounds().getCenter().y));
}

void PlayingHeader::draw(sf::RenderTarget &target,
                         sf::RenderStates states) const
{
    states.transform.translate(position);

    target.draw(header, states);
    target.draw(clockText, states);
    target.draw(remainingText, states);
    target.draw(smiley, states);
}

void PlayingHeader::startClock()
{
    clock.start();
}

void PlayingHeader::stopClock()
{
    clock.stop();
}

float PlayingHeader::getTime() const
{
    return clock.getElapsedTime().asSeconds();
}

void PlayingHeader::setRemainingMines(unsigned int remaining_)
{
    remaining = remaining_;
}

void PlayingHeader::setSmiley(sf::Texture const &texture)
{
    smiley.setTexture(texture);
    smiley.setScale(
        sf::Vector2f(SMILEY_SIZE.x / smiley.getLocalBounds().size.x,
                     SMILEY_SIZE.y / smiley.getLocalBounds().size.y));
    smiley.setOrigin(smiley.getLocalBounds().getCenter());
    smiley.setPosition(header.getSize() / 2.0f);
}

bool PlayingHeader::shouldRestart() const
{
    return restart;
}

sf::Vector2f PlayingHeader::getSize() const
{
    return header.getSize();
}

sf::Vector2f PlayingHeader::getPosition() const
{
    return position;
}
