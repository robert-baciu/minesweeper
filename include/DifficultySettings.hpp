#pragma once

class DifficultySettings
{
  public:
    class Builder;

    [[nodiscard]] int getCols() const;
    [[nodiscard]] int getRows() const;
    [[nodiscard]] float getMineDensity() const;

  private:
    DifficultySettings(int cols, int rows, float mineDensity);

    int cols;
    int rows;
    float mineDensity;
};

class DifficultySettings::Builder
{
  public:
    Builder() = default;

    Builder &withCols(int cols);
    Builder &withRows(int rows);
    Builder &withMineDensity(float mineDensity);

    [[nodiscard]] DifficultySettings build() const;

    int cols;
    int rows;
    float mineDensity;
};
