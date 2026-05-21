#include "PlaySettings.hpp"

PlaySettings::PlaySettings(int cols, int rows, float mineDensity)
    : cols(cols), rows(rows), mineDensity(mineDensity)
{
}

int PlaySettings::getCols() const
{
    return cols;
}

int PlaySettings::getRows() const
{
    return rows;
}

float PlaySettings::getMineDensity() const
{
    return mineDensity;
}

PlaySettings::Builder &PlaySettings::Builder::withCols(int cols)
{
    this->cols = cols;
    return *this;
}

PlaySettings::Builder &PlaySettings::Builder::withRows(int rows)
{
    this->rows = rows;
    return *this;
}

PlaySettings::Builder &PlaySettings::Builder::withMineDensity(float mineDensity)
{
    this->mineDensity = mineDensity;
    return *this;
}

PlaySettings PlaySettings::Builder::build() const
{
    return PlaySettings{cols, rows, mineDensity};
}
