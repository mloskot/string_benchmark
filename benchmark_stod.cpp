#define CELERO_STATIC
#include <algorithm>
#include <iostream>
#include "benchmark.hpp"
#include "fixture.hpp"

CELERO_MAIN

STRING_BASELINE(stod, stream)
{
    celero::DoNotOptimizeAway(fixture::to_number<double>(s_double));
}

STRING_BENCHMARK(stod, stod)
{
    celero::DoNotOptimizeAway(std::stod(s_double));
}

STRING_BENCHMARK(stod, stof)
{
    celero::DoNotOptimizeAway(std::stof(s_double));
}

STRING_BENCHMARK(stod, stold)
{
    celero::DoNotOptimizeAway(std::stold(s_double));
}
