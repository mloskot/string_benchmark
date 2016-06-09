#pragma once
#include <hayai.hpp>
#include <string>
#include "benchmark.hpp"

STRING_BENCHMARK(narrow_string, append)
{
    std::string result;

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    std::size_t test_size = result.size();
#endif

    for (auto const& s : string_benchmark::nstring_samples)
    {
        result.append(s);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        auto const new_size = result.size();
        assert(new_size == test_size + std::strlen(s));
        test_size = new_size;
#endif
    }
}

STRING_BENCHMARK(wide_string, append)
{
    std::wstring result;

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    std::size_t test_size = result.size();
#endif

    for (auto const& s : string_benchmark::wstring_samples)
    {
        result.append(s);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        auto const new_size = result.size();
        assert(new_size == test_size + std::wcslen(s));
        test_size = new_size;
#endif
    }
}
