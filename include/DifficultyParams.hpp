#pragma once

#include <optional>
#include <stdexcept>
#include <string>

class DifficultyParams
{
  public:
    class Builder;

    [[nodiscard]] int getCols() const;
    [[nodiscard]] int getRows() const;
    [[nodiscard]] float getMineDensity() const;

  private:
    DifficultyParams(int cols, int rows, float mineDensity);

    int const cols;
    int const rows;
    float const mineDensity;
};

class DifficultyParams::Builder
{
  private:
    std::optional<int> cols;
    std::optional<int> rows;
    std::optional<float> mineDensity;

  public:
    Builder &withCols(int cols_);
    Builder &withRows(int rows_);
    Builder &withMineDensity(float mineDensity_);

    [[nodiscard]] DifficultyParams build() const;
};

class DifficultyParamsBuilderError : public std::runtime_error
{
  public:
    explicit DifficultyParamsBuilderError(std::string const &field)
        : std::runtime_error(
              std::string("DifficultyParams builder is missing field \"") +
              field + std::string("\""))
    {
    }
};
