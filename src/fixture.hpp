#pragma once
#include <array>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

#ifndef STRING_BENCHMARK_ENABLE_STD_SPRINTF
#  define STRING_BENCHMARK_ENABLE_STD_SPRINTF 1
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

using n10strings = std::array<char const*, 10>;
using w10strings = std::array<wchar_t const*, 10>;

extern n10strings n10string_samples;
extern w10strings w10string_samples;

template <typename Char, template<typename> class Fixture>
struct base_fixture
{
    using string = std::basic_string<Char, std::char_traits<Char>>;
    using fixture = Fixture<Char>;

    static void sprintf_v(string& dst, Char const* const fmt, va_list args)
    {
        auto const ni = fixture::vscprintf(fmt, args);
        if (ni > 0)
        {
            auto const pos = dst.size();
            auto const add = static_cast<size_t>(ni);
            dst.resize(pos + add);

            auto const no = fixture::vsnprintf(&dst[pos], add + 1, fmt, args);
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
struct fixture;

template <>
struct fixture<char> : public base_fixture<char, fixture>
{
    using string = std::string;

    static auto samples() -> n10strings const&
    {
        return n10string_samples;
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

    static std::size_t vscprintf(char const* format, va_list args) PRINTF_FORMAT(1, 0)
    {
#ifdef STRING_BENCHMARK_ENABLE_STD_SPRINTF
        auto const n = std::vsnprintf(nullptr, 0, format, args);
#else
        auto const n = _vscprintf(format, args);
#endif
        return n;
    }

    static std::size_t vsnprintf(char* buffer, std::size_t buffer_size, char const* format, va_list args) PRINTF_FORMAT(3, 0)
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
struct fixture<wchar_t> : public base_fixture<wchar_t, fixture>
{
    static auto samples() -> w10strings const&
    {
        return w10string_samples;
    }

    static auto strlen(wchar_t const* const s) -> std::size_t
    {
        return std::wcslen(s);
    }

    static constexpr wchar_t const* const formatter_s()
    {
        return L"%s";
    }

    static constexpr wchar_t const* const formatter_s10()
    {
        return L"/%s/%s/%s/%s/%s/%s/%s/%s/%s/%s";
    }

    static std::size_t vscprintf(wchar_t const* format, va_list args)
    {
#ifdef STRING_BENCHMARK_ENABLE_STD_SPRINTF
        auto const n = std::vswprintf(nullptr, 0, format, args);
#else
        auto const n = _vscwprintf(format, args);
#endif
        return n;
    }

    static std::size_t vsnprintf(wchar_t* buffer, std::size_t buffer_size, wchar_t const* format, va_list args)
    {
#ifdef STRING_BENCHMARK_ENABLE_STD_SPRINTF
        auto const n = std::vswprintf(buffer, buffer_size, format, args);
#else
        auto const n = vswprintf_s(buffer, buffer_size, format, args);
#endif
        return n;
    }
};
