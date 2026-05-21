#include "AssetManager.hpp"

void AssetManager::load()
{
    fonts.add("main", "assets/fonts/VCR_OSD_MONO_1.001.ttf");
    textures.add("flag", "assets/textures/flag.png");
}

sf::Font const &AssetManager::getFont(std::string const &name) const
{
    return fonts.get(name);
}

sf::Font const &AssetManager::getMainFont() const
{
    return fonts.get("main");
}

sf::Texture const &AssetManager::getTexture(std::string const &name) const
{
    return textures.get(name);
}

std::ostream &operator<<(std::ostream &os, AssetManager const &assetManager)
{
    os << "AssetManager[fonts=" << assetManager.fonts << "]";
    return os;
}
