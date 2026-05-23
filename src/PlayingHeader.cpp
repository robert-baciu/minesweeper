#include "PlayingHeader.hpp"

#include "SFML/Graphics/RectangleShape.hpp"

PlayingHeader::PlayingHeader(State::Context const &ctx)
    : headerRect{
          {static_cast<float>(ctx.getWindow().get().getSize().x), HEIGHT}}
{
}

void PlayingHeader::draw(sf::RenderTarget &target,
                         sf::RenderStates states) const
{
    target.draw(headerRect, states);
}
