#include "PlayingHeader.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

PlayingHeader::PlayingHeader(StateCtxPtr const &ctx)
    : clockText(ctx->getAssets().getMainFont(), "0"),
      remainingMines(0),
      remainingText(ctx->getAssets().getMainFont(), "0"),
      smiley(ctx->getAssets().getTexture("smiley-playing"))
{
    header.setFillColor(HEADER_COLOR);

    clock.reset();

    clockText.setCharacterSize(PlayingGrid::CELL_SIZE);
    remainingText.setCharacterSize(PlayingGrid::CELL_SIZE);

    smiley.setScale(
        sf::Vector2f(SMILEY_SIZE.x / smiley.getLocalBounds().size.x,
                     SMILEY_SIZE.y / smiley.getLocalBounds().size.y));
}

void PlayingHeader::setHeaderSize(sf::Vector2f size)
{
    header.setSize(size);

    clockText.setPosition(
        sf::Vector2f(PlayingGrid::CELL_SIZE, header.getSize().y / 2.0f));
    clockText.setFillColor(TEXT_COLOR);

    remainingText.setPosition(
        sf::Vector2f(header.getSize().x - PlayingGrid::CELL_SIZE,
                     header.getSize().y / 2.0f));
    remainingText.setFillColor(TEXT_COLOR);

    smiley.setOrigin(smiley.getLocalBounds().getCenter());
    smiley.setPosition(size / 2.0f);
}

void PlayingHeader::setRemainingMines(int remaining)
{
    remainingMines = remaining;
}

sf::Sprite const &PlayingHeader::getSmiley()
{
    return smiley;
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

void PlayingHeader::draw(sf::RenderTarget &target,
                         sf::RenderStates states) const
{
    target.draw(header, states);
    target.draw(clockText, states);
    target.draw(remainingText, states);
    target.draw(smiley, states);
}

void PlayingHeader::update()
{
    clockText.setString(
        std::to_string(static_cast<int>(clock.getElapsedTime().asSeconds())));
    clockText.setOrigin(
        sf::Vector2f(0.0f, clockText.getLocalBounds().getCenter().y));

    remainingText.setString(std::to_string(remainingMines));
    remainingText.setOrigin(
        sf::Vector2f(remainingText.getLocalBounds().size.x,
                     remainingText.getLocalBounds().getCenter().y));
}

void PlayingHeader::startClock()
{
    clock.start();
}
