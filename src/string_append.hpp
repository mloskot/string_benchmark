#pragma once
#include <hayai.hpp>
#include <string>
#include "samples.hpp"

BENCHMARK(narrow_string, append, 10, 1000)
{
    std::string result;
    for (auto const& s : string_benchmark::nstring_samples)
        result.append(s);
}

BENCHMARK(wide_string, append, 10, 1000)
{
    std::wstring result;
    for (auto const& s : string_benchmark::wstring_samples)
        result.append(s);
}
