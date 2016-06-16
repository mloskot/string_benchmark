#pragma once

//#define STRING_BENCHMARK_ENABLE_TESTS
#ifndef NDEBUG
#define STRING_BENCHMARK_ENABLE_TESTS
#endif

#ifdef STRING_BENCHMARK_ENABLE_TESTS
#include <cassert>
#endif

namespace config
{
#ifdef NDEBUG
enum { runs = 1000, iterations = 1000 };
#else
enum { runs = 1, iterations = 1 };
#endif
}

#define STRING_BENCHMARK(fixture_name, benchmark_name) \
    BENCHMARK(fixture_name, benchmark_name, (config::runs), (config::iterations))
