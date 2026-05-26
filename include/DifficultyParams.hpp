#pragma once

class DifficultyParams
{
  public:
    class Builder;

    [[nodiscard]] int getCols() const;
    [[nodiscard]] int getRows() const;
    [[nodiscard]] float getMineDensity() const;

  private:
    DifficultyParams(int cols, int rows, float mineDensity);

    int cols;
    int rows;
    float mineDensity;
};

class DifficultyParams::Builder
{
  public:
    Builder() = default;

    Builder &withCols(int cols);
    Builder &withRows(int rows);
    Builder &withMineDensity(float mineDensity);

    [[nodiscard]] DifficultyParams build() const;

    int cols;
    int rows;
    float mineDensity;
};
