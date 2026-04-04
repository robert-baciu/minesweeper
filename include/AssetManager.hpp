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

void AssetManager::init()
{
    fonts.add("main", "assets/fonts/VCR_OSD_MONO_1.001.ttf");
}

sf::Font const &AssetManager::getFont(std::string const &name) const
{
    return fonts.get(name);
}

std::ostream &operator<<(std::ostream &os, AssetManager const &assetManager)
{
    os << "AssetManager[fonts=" << assetManager.fonts << "]";
    return os;
}
