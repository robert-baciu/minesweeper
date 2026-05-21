#pragma once

#include <memory>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <unordered_map>

#include "AssetLoadError.hpp"

template <typename T> class AssetCollection
{
  public:
    AssetCollection() = default;

    AssetCollection(AssetCollection const &other);

    AssetCollection &operator=(AssetCollection const &other);

    ~AssetCollection() = default;

    T const &add(std::string const &name, std::string const &path);

    T const &get(std::string const &name) const;

    friend std::ostream &operator<<(std::ostream &os,
                                    AssetCollection<T> const &assetCollection)
    {
        os << "AssetCollection[" << assetCollection.collection.size() << "]";
        return os;
    }

  private:
    std::unordered_map<std::string, std::shared_ptr<T>> collection;
};

template <typename T>
AssetCollection<T>::AssetCollection(AssetCollection<T> const &other)
    : collection(other.collection)
{
}

template <typename T>
AssetCollection<T> &
AssetCollection<T>::operator=(AssetCollection<T> const &other)
{
    AssetCollection temp(other);
    std::swap(collection, temp.collection);
    return *this;
}

template <typename T>
T const &AssetCollection<T>::add(std::string const &name,
                                 std::string const &path)
{
    auto asset = std::make_unique<T>();

    bool success;
    if constexpr (std::is_same_v<T, sf::Font>)
    {
        success = asset->openFromFile(path);
    }
    else
    {
        // Maybe, maybe, maybe...
        success = asset->loadFromFile(path);
    }

    if (!success)
    {
        throw AssetLoadError(path, name);
    }

    collection[name] = std::move(asset);
    return *collection.at(name);
}

template <typename T>
T const &AssetCollection<T>::get(std::string const &name) const
{
    return *collection.at(name);
}
