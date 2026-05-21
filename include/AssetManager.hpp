#pragma once

#include <SFML/Graphics/Font.hpp>

#include "AssetCollection.hpp"

class AssetManager
{
  public:
    void load();

    sf::Font const &getFont(std::string const &name) const;
    sf::Font const &getMainFont() const;

    sf::Texture const &getTexture(std::string const &name) const;

    friend std::ostream &operator<<(std::ostream &os,
                                    AssetManager const &assetManager);

  private:
    AssetCollection<sf::Font> fonts;
    AssetCollection<sf::Texture> textures;
};
