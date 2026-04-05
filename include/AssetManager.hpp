#pragma once

#include <SFML/Graphics/Font.hpp>

#include "AssetCollection.hpp"

class AssetManager
{
  public:
    void init();
    sf::Font const &getFont(std::string const &name) const;

    friend std::ostream &operator<<(std::ostream &os,
                                    AssetManager const &assetManager);

  private:
    AssetCollection<sf::Font> fonts;
};
