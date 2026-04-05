#pragma once

#include <memory>
#include <string>
#include <unordered_map>

template <typename T> class AssetCollection
{
  public:
    T const &add(std::string const &name, std::string const &filepath)
    {
        auto asset = std::make_unique<T>();
        if (!asset->openFromFile(filepath))
        {
            throw std::runtime_error("Could not load asset \"" + name +
                                     "\": " + filepath);
        }
        collection[name] = std::move(asset);
        return *collection.at(name);
    }

    T const &get(std::string const &name) const
    {
        return *collection.at(name);
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    AssetCollection const &assetCollection)
    {
        os << "AssetCollection[" << assetCollection.collection.size() << "]";
        return os;
    }

  private:
    std::unordered_map<std::string, std::unique_ptr<T>> collection;
};
