#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <random>
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

template<typename CharT>
struct xml
{
    static_assert(sizeof(CharT) == -1, "CharT must be char or wchar_t");
};

template<>
struct xml<char>
{
    static constexpr char const* amp                  = "&";
    static constexpr char const* apos                 = "\'";
    static constexpr char const* backslash            = "\\";
    static constexpr char const* cr                   = "\r";
    static constexpr char const* gt                   = ">";
    static constexpr char const* lf                   = "\n";
    static constexpr char const* lt                   = "<";
    static constexpr char const* question             = "?";
    static constexpr char const* quot                 = "\"";
    static constexpr char const* tab                  = "\t";
    static constexpr char const* entity_amp           = "&amp;";
    static constexpr char const* entity_apos          = "&apos;";
    static constexpr char const* entity_gt            = "&gt;";
    static constexpr char const* entity_lt            = "&lt;";
    static constexpr char const* entity_quot          = "&quot;";
    static constexpr char const* entity_unknown       = "?";
    static constexpr char const* escape_set_attribute = "\'\"&";
    static constexpr char const* escape_set_text      = "&<>";
};

template<>
struct xml<wchar_t>
{
    static constexpr wchar_t const* amp                  = L"&";
    static constexpr wchar_t const* apos                 = L"\'";
    static constexpr wchar_t const* backslash            = L"\\";
    static constexpr wchar_t const* cr                   = L"\r";
    static constexpr wchar_t const* gt                   = L">";
    static constexpr wchar_t const* lf                   = L"\n";
    static constexpr wchar_t const* lt                   = L"<";
    static constexpr wchar_t const* question             = L"?";
    static constexpr wchar_t const* quot                 = L"\"";
    static constexpr wchar_t const* tab                  = L"\t";
    static constexpr wchar_t const* entity_amp           = L"&amp;";
    static constexpr wchar_t const* entity_apos          = L"&apos;";
    static constexpr wchar_t const* entity_gt            = L"&gt;";
    static constexpr wchar_t const* entity_lt            = L"&lt;";
    static constexpr wchar_t const* entity_quot          = L"&quot;";
    static constexpr wchar_t const* entity_unknown       = L"?";
    static constexpr wchar_t const* escape_set_attribute = L"\'\"&";
    static constexpr wchar_t const* escape_set_text      = L"&<>";
};

template <typename Char, template<typename> class Fixture>
struct base_string_fixture
{
    using string = std::basic_string<Char, std::char_traits<Char>>;
    using fixture = Fixture<Char>;

    static bool starts_with(Char const* const s, Char const* const n)
    {
        auto const s_len = fixture::strlen(s);
        auto const n_len = fixture::strlen(n);
        if (s_len < n_len)
            return false;
        return fixture::strncmp(s, n, n_len) == 0;
    }

    static bool starts_with(string const& s, string const& n)
    {
        auto const n_len = n.length();
        if (s.length() < n_len)
            return false;
        return fixture::strncmp(s.c_str(), n.c_str(), n_len) == 0;
    }

    static bool ends_with(Char const* const s, Char const* const n)
    {
        auto const s_len = fixture::strlen(s);
        auto const n_len = fixture::strlen(n);
        if (s_len < n_len)
            return false;
        return fixture::strncmp(s + (s_len - n_len), n, n_len) == 0;
    }

    static bool ends_with(string const& s, string const& n)
    {
        auto const s_len = s.length();
        auto const n_len = n.length();
        if (s_len < n_len)
            return false;
        return fixture::strncmp(s.c_str() + (s_len - n_len), n.c_str(), n_len) == 0;
    }

    static bool istarts_with(Char const* const s, Char const* const n)
    {
        auto const s_len = fixture::strlen(s);
        auto const n_len = fixture::strlen(n);
        if (s_len < n_len)
            return false;
        return fixture::strnicmp(s, n, n_len) == 0;
    }

    static bool istarts_with(string const& s, string const& n)
    {
        auto const n_len = n.length();
        if (s.length() < n_len)
            return false;
        return fixture::strnicmp(s.c_str(), n.c_str(), n_len) == 0;
    }

    static bool iends_with(Char const* const s, Char const* const n)
    {
        auto const s_len = fixture::strlen(s);
        auto const n_len = fixture::strlen(n);
        if (s_len < n_len)
            return false;
        return fixture::strnicmp(s + (s_len - n_len), n, n_len) == 0;
    }

    static bool iends_with(string const& s, string const& n)
    {
        auto const s_len = s.length();
        auto const n_len = n.length();
        if (s_len < n_len)
            return false;
        return fixture::strnicmp(s.c_str() + (s_len - n_len), n.c_str(), n_len) == 0;
    }

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

    static auto sprintf(_Printf_format_string_ Char const* const fmt, ...) -> string
    {
        string dst;
        va_list args;
        va_start(args, fmt);
        fixture::sprintf_v(dst, fmt, args);
        va_end(args);
        return dst;
    }

    static auto replace(string& str, Char const* search, Char const* replace) -> string&
    {
        std::size_t pos;
        auto const replace_len = fixture::strlen(replace);
        for (pos = str.find_first_of(search);
             pos != string::npos;
             pos = str.find_first_of(search, pos))
        {
            str.replace(pos, 1, replace);
            pos += replace_len - 1;
        }
        return str;
    }

    static auto escape_xml(string& str) -> string&
    {
        using xml = xml<Char>;
        std::size_t pos, n{0};
        for (pos = str.find_first_of(xml::escape_set_text);
            pos != string::npos;
            pos = str.find_first_of(xml::escape_set_text, pos))
        {
            Char const* entity{nullptr};
            switch (str[pos])
            {
            case xml::amp[0]: entity = xml::entity_amp; n = 5; break;
            case xml::lt[0]:  entity = xml::entity_lt;  n = 4; break;
            case xml::gt[0]:  entity = xml::entity_gt;  n = 4; break;
            default:          entity = xml::question;   n=1;   break;
            }
            str.replace(pos, 1, entity);
            pos += n - 1; // add n, remove 1.
        }
        return str;
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

    static auto strncmp(char const* const lhs, char const* const rhs, std::size_t count) -> int
    {
        return std::strncmp(lhs, rhs, count);
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

    static auto strncmp(wchar_t const* const lhs, wchar_t const* const rhs, std::size_t count) -> int
    {
        return std::wcsncmp(lhs, rhs, count);
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
inline auto make_random_string(std::size_t size, std::basic_string<Char> const& alphabet) -> std::basic_string<Char>
{
    std::mt19937_64 gen{std::random_device()()};
    std::uniform_int_distribution<size_t> dist{0, alphabet.length() - 1};
    std::basic_string<Char> ret;
    std::generate_n(std::back_inserter(ret), size, [&] { return alphabet[dist(gen)]; });
    return ret;
}

template <typename Char>
inline auto random_istring(std::size_t size) -> std::basic_string<Char>
{
    std::basic_string<Char> alphabet;
    alphabet.reserve(('Z' - 'A') + ('z' - 'a')+ 1);
    for (Char c = 'a'; c <= 'z'; ++c)
        alphabet.push_back(c);
    for (Char c = 'A'; c <= 'Z'; ++c)
        alphabet.push_back(c);
    return make_random_string(size, alphabet);
}

template <typename Char>
inline auto random_string(std::size_t size) -> std::basic_string<Char>
{
    std::basic_string<Char> alphabet;
    alphabet.reserve(('z' - 'a')+ 1);
    for (Char c = 'a'; c <= 'z'; ++c)
        alphabet.push_back(c);
    return make_random_string(size, alphabet);
}

template <typename Char>
inline auto random_xml(std::size_t size) -> std::basic_string<Char>
{
    using xml = xml<Char>;
    std::basic_string<Char> alphabet;
    alphabet.push_back(xml::amp[0]);
    alphabet.push_back(xml::apos[0]);
    alphabet.push_back(xml::lt[0]);
    alphabet.push_back(xml::gt[0]);
    alphabet.push_back(xml::quot[0]);
    return make_random_string(size, alphabet);
}

template <typename Char>
struct  data_fixture : celero::TestFixture
{
    using fixture = string_fixture<Char>;
    using string = std::basic_string<Char>;
    string s1;
    string s2;
    string si1; // case-insensitive
    string si2; // case-insensitive
    string sxml;

    auto getExperimentValues() const -> std::vector<std::pair<int64_t, uint64_t>>
    {
        std::vector<std::pair<int64_t, uint64_t>> v;
        v.emplace_back(0, 0);
        std::int64_t n{0};
        std::generate_n(std::back_inserter(v), 10,
            [&n]() -> std::pair<int64_t, uint64_t> { return {n += 10, 0}; });
        v.emplace_back(512, 0);
        v.emplace_back(1024, 0);
        return v;
    }

    void setUp(int64_t experimentValue) final
    {
        auto const size = static_cast<std::size_t>(experimentValue);
        s1 = s2 = random_string<Char>(size);
        si1 = si2 = random_istring<Char>(size);
        sxml = random_xml<Char>(size);
    }
};

}
