#pragma once
#include "samples.hpp"

//#define STRING_BENCHMARK_ENABLE_TESTS
#ifndef NDEBUG
#define STRING_BENCHMARK_ENABLE_TESTS
#endif

#ifdef STRING_BENCHMARK_ENABLE_TESTS
#include <cassert>
#endif


namespace string_benchmark
{
#ifdef NDEBUG
enum { runs = 100, iterations = 10000 };
#else
enum { runs = 1, iterations = 1 };
#endif
}

#define STRING_BENCHMARK(fixture_name, benchmark_name) \
    BENCHMARK(fixture_name, benchmark_name, (string_benchmark::runs), (string_benchmark::iterations))
