#pragma once
#include <array>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#include <vector>
#include <strings.h> // strncasecmp
#include <wchar.h>   // wcsncasecmp
#endif
#include <string>
#include "benchmark.hpp"

#ifndef STRING_BENCHMARK_ENABLE_STD_SPRINTF
#define STRING_BENCHMARK_ENABLE_STD_SPRINTF 1
#endif

// Source: Chromium
// Tell the compiler a function is using a printf-style format string.
// |format_param| is the one-based index of the format string parameter;
// |dots_param| is the one-based index of the "..." parameter.
// For v*printf functions (which take a va_list), pass 0 for dots_param.
#ifdef _MSC_VER
#define PRINTF_FORMAT(format_param, dots_param)
#else
// FIXME: does not work for wchar_t
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=38308
#define PRINTF_FORMAT(format_param, dots_param) \
__attribute__((format(printf, format_param, dots_param)))
#define _Printf_format_string_
#endif

namespace string_benchmark
{

using n10strings = std::array<char const*, 10>;
using w10strings = std::array<wchar_t const*, 10>;

extern n10strings n10string_samples;
extern w10strings w10string_samples;

template <typename Char, template<typename> class Fixture>
struct base_string_fixture
{
    using string = std::basic_string<Char, std::char_traits<Char>>;
    using fixture = Fixture<Char>;

    static void sprintf_v(string& dst, Char const* const fmt, va_list args)
    {
        va_list args2;
        va_copy(args2, args);
        auto const ni = fixture::vscprintf(fmt, args);
        if (ni > 0)
        {

            auto const pos = dst.size();
            auto const add = static_cast<size_t>(ni);
            dst.resize(pos + add);

            auto const no = fixture::vsnprintf(&dst[pos], add + 1, fmt, args2);
            va_end(args2);
            if (no != ni)
            {
                assert(0);
                dst.resize(pos);
            }
        }
    }

    static string sprintf(_Printf_format_string_ Char const* const fmt, ...)
    {
        string dst;
        va_list args;
        va_start(args, fmt);
        fixture::sprintf_v(dst, fmt, args);
        va_end(args);
        return dst;
    }
};

template <typename Char>
struct string_fixture;

template <>
struct string_fixture<char> : public base_string_fixture<char, string_fixture>
{
    using string = std::string;

    static auto samples() -> n10strings const&
    {
        return n10string_samples;
    }

    static auto strnicmp(char const* const lhs, char const* const rhs, std::size_t count) -> int
    {
#ifdef _MSC_VER
        return ::_strnicmp(lhs, rhs, count);
#else
        return ::strncasecmp(lhs, rhs, count);
#endif
    }

    static auto strlen(char const* const s) -> std::size_t
    {
        return std::strlen(s);
    }

    static constexpr char const* const formatter_s()
    {
        return "%s";
    }

    static constexpr char const* const formatter_s10()
    {
        return "/%s/%s/%s/%s/%s/%s/%s/%s/%s/%s";
    }

    static int vscprintf(char const* format, va_list args) PRINTF_FORMAT(1, 0)
    {
#ifdef STRING_BENCHMARK_ENABLE_STD_SPRINTF
        auto const n = std::vsnprintf(nullptr, 0, format, args);
#else
        auto const n = _vscprintf(format, args);
#endif
        return n;
    }

    static int vsnprintf(char* buffer, std::size_t buffer_size, char const* format, va_list args) PRINTF_FORMAT(3, 0)
    {
#ifdef STRING_BENCHMARK_ENABLE_STD_SPRINTF
        auto const n = std::vsnprintf(buffer, buffer_size, format, args);
#else
        auto const n = vsprintf_s(buffer, buffer_size, format, args);
#endif
        return n;
    }
};

template <>
struct string_fixture<wchar_t> : public base_string_fixture<wchar_t, string_fixture>
{
    static auto samples() -> w10strings const&
    {
        return w10string_samples;
    }

    static auto strnicmp(wchar_t const* const lhs, wchar_t const* const rhs, std::size_t count) -> int
    {
#ifdef _MSC_VER
        return ::_wcsnicmp(lhs, rhs, count);
#else
        return ::wcsncasecmp(lhs, rhs, count);
#endif
    }

    static auto strlen(wchar_t const* const s) -> std::size_t
    {
        return std::wcslen(s);
    }

    static constexpr wchar_t const* const formatter_s()
    {
        return L"%ls";
    }

    static constexpr wchar_t const* const formatter_s10()
    {
        return L"/%ls/%ls/%ls/%ls/%ls/%ls/%ls/%ls/%ls/%ls";
    }

    static int vscprintf(wchar_t const* format, va_list args)
    {
#ifdef _MSC_VER
        return _vscwprintf(format, args);
#else
        int n = 1024 ;
        while (n < 1024 * 1024)
        {
            va_list args1;
            va_copy(args1, args);
            std::vector<wchar_t> buffer(n);
            auto const fmt_n = vswprintf(buffer.data(), buffer.size()+1, format, args1);
            va_end(args1);
            if (fmt_n >= 0)
                return fmt_n;
            n *= 2;
        }
        return -1;
#endif
    }

    static int vsnprintf(wchar_t* buffer, std::size_t buffer_size, wchar_t const* format, va_list args)
    {
#ifdef STRING_BENCHMARK_ENABLE_STD_SPRINTF
        auto const n = std::vswprintf(buffer, buffer_size, format, args);
#else
        auto const n = vswprintf_s(buffer, buffer_size, format, args);
#endif
        return n;
    }
};

template <typename Char>
struct  data_fixture : celero::TestFixture
{
    using fixture = string_fixture<Char>;
    using string = std::basic_string<Char>;
    string s1;
    string s2;

    auto getExperimentValues() const -> std::vector<std::pair<int64_t, uint64_t>>
    {
        std::vector<std::pair<int64_t, uint64_t>> v(fixture::samples().size());
        std::int64_t n{0};
        std::generate(v.begin(), v.end(),
            [&n]() -> std::pair<int64_t, uint64_t> { return {n += 10, 0}; });
        return v;
    }

    void setUp(int64_t experimentValue) final
    {
        auto const& samples = fixture::samples();
        assert(experimentValue % 10 == 0);
        auto const i = static_cast<std::size_t>(experimentValue / 10)  -1;
        assert(i < samples.size());
        s1 = s2 = samples[i];
    }
};

}