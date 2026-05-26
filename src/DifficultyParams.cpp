#include "DifficultyParams.hpp"

DifficultyParams::DifficultyParams(int cols, int rows, float mineDensity)
    : cols(cols),
      rows(rows),
      mineDensity(mineDensity)
{
}

int DifficultyParams::getCols() const
{
    return cols;
}

int DifficultyParams::getRows() const
{
    return rows;
}

float DifficultyParams::getMineDensity() const
{
    return mineDensity;
}

DifficultyParams::Builder &DifficultyParams::Builder::withCols(int cols)
{
    this->cols = cols;
    return *this;
}

DifficultyParams::Builder &DifficultyParams::Builder::withRows(int rows)
{
    this->rows = rows;
    return *this;
}

DifficultyParams::Builder &
DifficultyParams::Builder::withMineDensity(float mineDensity)
{
    this->mineDensity = mineDensity;
    return *this;
}

DifficultyParams DifficultyParams::Builder::build() const
{
    return DifficultyParams{cols, rows, mineDensity};
}
