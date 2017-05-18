#pragma once
#include <hayai.hpp>
#include "benchmark.hpp"
#include "fixture.hpp"
#ifdef HAS_BOOST
#include <boost/algorithm/string/predicate.hpp>
#endif
#include <iostream>

template <typename Char>
struct test_starts_with
{
    using f = fixture<Char>;
    using string = typename fixture<Char>::string;

    static void strnicmp_first_chars10(Char const* const s, Char const* const needle)
    {
        int r = f::strnicmp(s, needle, 10);
        (void)r;

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        assert(r == 0);
#endif
    }

    static void strnicmp_chars10_search10()
    {
        string const n = f::samples().front();
        string const s = f::samples().front();
        strnicmp_first_chars10(s.c_str(), n.c_str());
    }

    static void strnicmp_chars100_search10()
    {
        string const n = f::samples().front();
        string const s = f::samples().back();
        strnicmp_first_chars10(s.c_str(), n.c_str());
    }

#ifdef HAS_BOOST
    static void boost_istarts_with(string const& s, string const& needle)
    {
        bool r = boost::istarts_with(s, needle);
        (void)r;

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        assert(r);
#endif
    }

    static void boost_istarts_with_chars10_search10()
    {
        string const n = f::samples().front();
        string const s = f::samples().front();
        boost_istarts_with(s, n);
    }

    static void boost_istarts_with_chars100_search10()
    {
        string const n = f::samples().front();
        string const s = f::samples().back();
        boost_istarts_with(s, n);
    }
#endif // HAS_BOOST

};

// strnicmp ////////////////////////////////////////////////////////////////////

STRING_BENCHMARK(string, strnicmp_chars10_search10)
{
    test_starts_with<char>::strnicmp_chars10_search10();
}

STRING_BENCHMARK(string, strnicmp_chars100_search10)
{
    test_starts_with<char>::strnicmp_chars100_search10();
}

STRING_BENCHMARK(wstring, strnicmp_chars10_search10)
{
    test_starts_with<wchar_t>::strnicmp_chars10_search10();
}

STRING_BENCHMARK(wstring, strnicmp_chars100_search10)
{
    test_starts_with<wchar_t>::strnicmp_chars100_search10();
}

// Boost ///////////////////////////////////////////////////////////////////////
#ifdef HAS_BOOST

STRING_BENCHMARK(string, boost_istarts_with_chars10_search10)
{
    test_starts_with<char>::boost_istarts_with_chars10_search10();
}

STRING_BENCHMARK(string, boost_istarts_with_chars100_search10)
{
    test_starts_with<char>::boost_istarts_with_chars100_search10();
}

STRING_BENCHMARK(wstring, boost_istarts_with_chars10_search10)
{
    test_starts_with<wchar_t>::boost_istarts_with_chars10_search10();
}

STRING_BENCHMARK(wstring, boost_istarts_with_chars100_search10)
{
    test_starts_with<wchar_t>::boost_istarts_with_chars100_search10();
}

#endif // HAS_BOOST
