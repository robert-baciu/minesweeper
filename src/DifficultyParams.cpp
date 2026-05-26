#include "DifficultyParams.hpp"

#include <string>

DifficultyParams::DifficultyParams(int cols, int rows, float mineDensity)
    : cols(cols),
      rows(rows),
      mineDensity(mineDensity)
{
}

// cppcheck-suppress unusedFunction
int DifficultyParams::getCols() const
{
    return cols;
}

// cppcheck-suppress unusedFunction
int DifficultyParams::getRows() const
{
    return rows;
}

// cppcheck-suppress unusedFunction
float DifficultyParams::getMineDensity() const
{
    return mineDensity;
}

// cppcheck-suppress unusedFunction
DifficultyParams::Builder &DifficultyParams::Builder::withCols(int cols_)
{
    cols = cols_;
    return *this;
}

// cppcheck-suppress unusedFunction
DifficultyParams::Builder &DifficultyParams::Builder::withRows(int rows_)
{
    rows = rows_;
    return *this;
}

DifficultyParams::Builder &
// cppcheck-suppress unusedFunction
DifficultyParams::Builder::withMineDensity(float mineDensity_)
{
    mineDensity = mineDensity_;
    return *this;
}

// cppcheck-suppress unusedFunction
DifficultyParams DifficultyParams::Builder::build() const
{
    if (!cols.has_value())
    {
        throw DifficultyParamsBuilderError(std::string("cols"));
    }

    if (!rows.has_value())
    {
        throw DifficultyParamsBuilderError(std::string("rows"));
    }

    if (!mineDensity.has_value())
    {
        throw DifficultyParamsBuilderError(std::string("mineDensity"));
    }

    return {cols.value(), rows.value(), mineDensity.value()};
}
