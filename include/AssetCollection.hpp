#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

template <typename T> class AssetCollection
{
  public:
    T const &add(std::string const &name, std::string const &filepath);
    T const &get(std::string const &name) const;

    template <typename Type>
    friend std::ostream &
    operator<<(std::ostream &os, AssetCollection<Type> const &assetCollection);

  private:
    std::unordered_map<std::string, std::unique_ptr<T>> collection;
};

template <typename T>
T const &AssetCollection<T>::add(std::string const &name,
                                 std::string const &path)
{
    auto asset = std::make_unique<T>();
    if (!asset->openFromFile(path))
    {
        throw std::runtime_error("Could not load asset \"" + name +
                                 "\": " + path);
    }
    collection[name] = std::move(asset);
    return *collection.at(name);
}

template <typename T>
T const &AssetCollection<T>::get(std::string const &name) const
{
    return *collection.at(name);
}

template <typename T>
std::ostream &operator<<(std::ostream &os,
                         AssetCollection<T> const &assetCollection)
{
    os << "AssetCollection[" << assetCollection.collection.size() << "]";
    return os;
}
