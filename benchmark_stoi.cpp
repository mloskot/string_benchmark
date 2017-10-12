#define CELERO_STATIC
#include <algorithm>
#include <iostream>
#include "benchmark.hpp"
#include "fixture.hpp"

CELERO_MAIN

STRING_BASELINE(stoi, stream)
{
    celero::DoNotOptimizeAway(fixture::to_number<int>(s_int));
}

STRING_BENCHMARK(stoi, stoi)
{
    celero::DoNotOptimizeAway(std::stoi(s_int));
}

STRING_BENCHMARK(stoi, stol)
{
    celero::DoNotOptimizeAway(std::stol(s_int));
}

STRING_BENCHMARK(stoi, stoll)
{
    celero::DoNotOptimizeAway(std::stoll(s_int));
}
