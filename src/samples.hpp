#pragma once
#include <array>

namespace string_benchmark
{

using nstrings = std::array<char const*, 10>;
using wstrings = std::array<wchar_t const*, 10>;

extern nstrings nstring_samples;
extern wstrings wstring_samples;

}