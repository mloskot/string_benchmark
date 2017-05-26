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

#ifndef NDEBUG
#include <cassert>
#endif

#ifdef STRING_BENCHMARK_WCHAR
#define STRING_CHAR_TYPE wchar_t
#define GROUP_NAME(group_name) w ## group_name
#else
#define STRING_CHAR_TYPE char
#define GROUP_NAME(group_name) group_name
#endif

namespace string_benchmark
{
namespace config
{

// If samples == 0, then the test is repeated until it as ran for at least one
// second or at least 30 samples have been taken.
#ifdef NDEBUG
enum { samples = 100, iterations = 1000 };
#else
enum { samples = 1, iterations = 10 };
#endif

}} // namespace string_benchmark::config

template <typename T>
inline constexpr void ignore_unused(T const&) {}

#define STRING_BASELINE(group_name, benchmark_name) \
    BASELINE_F(GROUP_NAME(group_name), benchmark_name, \
        string_benchmark::data_fixture<STRING_CHAR_TYPE>, \
        (string_benchmark::config::samples), \
        (string_benchmark::config::iterations))

#define STRING_BENCHMARK(group_name, benchmark_name) \
    BENCHMARK_F(GROUP_NAME(group_name), benchmark_name, \
        string_benchmark::data_fixture<STRING_CHAR_TYPE>, \
        (string_benchmark::config::samples), \
        (string_benchmark::config::iterations))
