#pragma once
#include <hayai.hpp>
#include <cassert>
#include <cstdio>
#include <string>
#include "samples.hpp"

namespace string_benchmark
{

// On Windows, prefer Microsoft-specific string I/O functions
// assuming they are best optimised for this platform.

inline void nstring_sprintf_v(std::string& dst, char const* fmt, va_list args)
{
#ifdef _MSC_VER
    auto const ni=_vscprintf(fmt, args);
#else
    auto const ni = std::vsnprintf(nullptr, 0, fmt, args);
#endif
    if (ni > 0)
    {
        auto const pos = dst.size();
        auto const add = static_cast<size_t>(ni);
        dst.resize(pos + add);
#ifdef _MSC_VER
        auto const no = vsprintf_s(&dst[pos], add + 1, fmt, args);
#else
        auto const no = vsnprintf(&dst[pos], add + 1, fmt, args);
#endif
        if (no != ni)
        {
            assert(0);
            dst.resize(pos);
        }
    }
}

inline void wstring_sprintf_v(std::wstring& dst, wchar_t const* fmt, va_list args)
{
#ifdef _MSC_VER
    auto const ni=_vscwprintf(fmt, args);
#else
    auto const ni = std::vswprintf(nullptr, 0, fmt, args);
#endif
    if (ni > 0)
    {
        auto const pos = dst.size();
        auto const add = static_cast<size_t>(ni);
        dst.resize(pos + add);
#ifdef _MSC_VER
        auto const no = vswprintf_s(&dst[pos], add + 1, fmt, args);
#else
        auto const no = vswprintf(&dst[pos], add + 1, fmt, args);
#endif
        if (no != ni)
        {
            assert(0);
            dst.resize(pos);
        }
    }
}

inline std::string nstring_sprintf(char const* fmt, ...)
{
    std::string dst;
    va_list args;
    va_start(args, fmt);
    nstring_sprintf_v(dst, fmt, args);
    va_end(args);
    return dst;
}

inline std::wstring wstring_sprintf(wchar_t const* fmt, ...)
{
    std::wstring dst;
    va_list args;
    va_start(args, fmt);
    wstring_sprintf_v(dst, fmt, args);
    va_end(args);
    return dst;
}

}

BENCHMARK(narrow_string, sprintf, 10, 1000)
{
    std::string result;
    for (auto const& s : string_benchmark::nstring_samples)
        result = string_benchmark::nstring_sprintf("head/%s/foot", s);
}

BENCHMARK(wide_string, sprintf, 10, 1000)
{
    std::wstring result;
    for (auto const& s : string_benchmark::wstring_samples)
        result = string_benchmark::wstring_sprintf(L"head/%s/foot", s);
}
