#include "DifficultySettings.hpp"

DifficultySettings::DifficultySettings(int cols, int rows, float mineDensity)
    : cols(cols),
      rows(rows),
      mineDensity(mineDensity)
{
}

int DifficultySettings::getCols() const
{
    return cols;
}

int DifficultySettings::getRows() const
{
    return rows;
}

float DifficultySettings::getMineDensity() const
{
    return mineDensity;
}

DifficultySettings::Builder &DifficultySettings::Builder::withCols(int cols)
{
    this->cols = cols;
    return *this;
}

DifficultySettings::Builder &DifficultySettings::Builder::withRows(int rows)
{
    this->rows = rows;
    return *this;
}

DifficultySettings::Builder &
DifficultySettings::Builder::withMineDensity(float mineDensity)
{
    this->mineDensity = mineDensity;
    return *this;
}

DifficultySettings DifficultySettings::Builder::build() const
{
    return DifficultySettings{cols, rows, mineDensity};
}
