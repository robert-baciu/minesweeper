#pragma once

#include "MinesweeperError.hpp"

class AssetLoadError : public MinesweeperError
{
  public:
    AssetLoadError(std::string const &path, std::string const &name)
        : MinesweeperError("AssetLoadError: Could not load asset named \"" +
                           name + "\" from path \"" + path + "\"")
    {
    }
};
