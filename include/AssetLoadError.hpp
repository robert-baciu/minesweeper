#pragma once

#include <stdexcept>

class AssetLoadError : public std::runtime_error
{
  public:
    AssetLoadError(std::string const &path, std::string const &name)
        : std::runtime_error("AssetLoadError: Could not load asset named \"" +
                             name + "\" from path \"" + path + "\"")
    {
    }
};
