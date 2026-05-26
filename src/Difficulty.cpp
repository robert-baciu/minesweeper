#include "Difficulty.hpp"

#include <utility>

std::string DifficultyUtil::toString(Difficulty difficulty)
{
    auto it = STRINGS.find(difficulty);
    if (it != STRINGS.end())
    {
        return it->second;
    }

    // TODO: Exception
    std::unreachable();
}

Difficulty DifficultyUtil::fromString(std::string const &string)
{
    for (auto const &[key, value] : STRINGS)
    {
        if (value == string)
        {
            return key;
        }
    }

    // TODO: Exception
    std::unreachable();
}

std::vector<Difficulty> DifficultyUtil::all()
{
    return std::vector<Difficulty>{
        Difficulty::Easy,    Difficulty::Medium, Difficulty::Hard,
        Difficulty::Extreme, Difficulty::Custom,
    };
}

std::vector<std::string> DifficultyUtil::allStrings()
{
    std::vector<std::string> strings;
    strings.reserve(STRINGS.size());

    for (auto const &[difficulty, name] : STRINGS)
    {
        strings.push_back(name);
    }
    return strings;
}
