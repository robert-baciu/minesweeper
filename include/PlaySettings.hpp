#pragma once

class PlaySettings
{
  public:
    class Builder;

    [[nodiscard]] int getCols() const;
    [[nodiscard]] int getRows() const;
    [[nodiscard]] float getMineDensity() const;

  private:
    PlaySettings(int cols, int rows, float mineDensity);

    int cols;
    int rows;
    float mineDensity;
};

class PlaySettings::Builder
{
  public:
    Builder() = default;

    Builder &withCols(int cols);
    Builder &withRows(int rows);
    Builder &withMineDensity(float mineDensity);

    [[nodiscard]] PlaySettings build() const;

    int cols;
    int rows;
    float mineDensity;
};
