#pragma once

#include <map>
#include <string>
#include <vector>

enum class Difficulty
{
    Easy,
    Medium,
    Hard,
    Extreme,
    Custom
};

class DifficultyUtil
{
  public:
    DifficultyUtil() = delete;

    static std::string toString(Difficulty difficulty);
    static Difficulty fromString(std::string const &string);

    static std::vector<Difficulty> all();
    static std::vector<std::string> allStrings();

  private:
    static inline std::map<Difficulty, std::string> const STRINGS = {
        {Difficulty::Easy, "Easy"},
        {Difficulty::Medium, "Medium"},
        {Difficulty::Hard, "Hard"},
        {Difficulty::Extreme, "Extreme"},
        {Difficulty::Custom, "Custom"}};
};
