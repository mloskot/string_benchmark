#define CELERO_STATIC
#include <algorithm>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>
#include "benchmark.hpp"
#include "fixture.hpp"

CELERO_MAIN

STRING_BASELINE(istarts_with, strnicmp_char10, char)
{
    auto const r = fixture::strnicmp(s1.c_str(), s2.c_str(), 10);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r == 0);
#endif
}

STRING_BENCHMARK(istarts_with, strnicmp_strlen, char)
{
    auto const r = fixture::strnicmp(s1.c_str(), s2.c_str(), fixture::strlen(s1.c_str()));

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r == 0);
#endif
}

#ifdef HAS_BOOST

STRING_BENCHMARK(istarts_with, boost, char)
{
    auto const r = boost::istarts_with(s1, s2);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r);

#endif
}

#endif // HAS_BOOST

STRING_BASELINE(istarts_with_w, strnicmp_char10, wchar_t)
{
    auto const r = fixture::strnicmp(s1.c_str(), s2.c_str(), 10);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r == 0);
#endif
}

STRING_BENCHMARK(istarts_with_w, strnicmp_strlen, wchar_t)
{
    auto const r = fixture::strnicmp(s1.c_str(), s2.c_str(), fixture::strlen(s1.c_str()));

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r == 0);
#endif
}

#ifdef HAS_BOOST

STRING_BENCHMARK(istarts_with_w, boost, wchar_t)
{
    auto const r = boost::istarts_with(s1, s2);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r);

#endif
}

#endif // HAS_BOOST