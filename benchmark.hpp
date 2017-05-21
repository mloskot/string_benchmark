#pragma once
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4244) // conversion, possible loss of data
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
#endif
#include <celero/Celero.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

//#define STRING_BENCHMARK_ENABLE_TESTS
#ifndef NDEBUG
#define STRING_BENCHMARK_ENABLE_TESTS
#endif

#ifdef STRING_BENCHMARK_ENABLE_TESTS
#include <cassert>
#endif

namespace string_benchmark
{
namespace config
{

#ifdef NDEBUG
enum { runs = 10, iterations = 1000 };
#else
enum { runs = 1, iterations = 10 };
#endif

}} // namespace string_benchmark::config

template <typename T>
inline constexpr void ignore_unused(T const&) {}

#define STRING_BASELINE(group_name, benchmark_name, char_type) \
    BASELINE_F(group_name, benchmark_name, \
        string_benchmark::data_fixture<char_type>, \
        (string_benchmark::config::runs), \
        (string_benchmark::config::iterations))

#define STRING_BENCHMARK(group_name, benchmark_name, char_type) \
    BENCHMARK_F(group_name, benchmark_name, \
        string_benchmark::data_fixture<char_type>, \
        (string_benchmark::config::runs), \
        (string_benchmark::config::iterations))
