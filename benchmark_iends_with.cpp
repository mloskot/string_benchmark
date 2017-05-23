#define CELERO_STATIC
#include <algorithm>
#include <iostream>
#ifdef HAS_BOOST
#include <boost/algorithm/string/predicate.hpp>
#endif
#include "benchmark.hpp"
#include "fixture.hpp"

CELERO_MAIN

STRING_BASELINE(iends_with, strnicmp)
{
    celero::DoNotOptimizeAway(fixture::strnicmp(s1.c_str(), s2.c_str(), s1.size()));

    assert(s1.size() == s1.size());
    assert(fixture::strnicmp(s1.c_str(), s2.c_str(), s1.size()) == 0);
}

STRING_BENCHMARK(iends_with, strnicmp_cstring)
{
    celero::DoNotOptimizeAway(fixture::iends_with(s1.c_str(), s2.c_str()));

    assert(fixture::iends_with(s1.c_str(), s2.c_str()));
}

STRING_BENCHMARK(iends_with, strnicmp_string)
{
    celero::DoNotOptimizeAway(fixture::iends_with(s1, s2));

    assert(fixture::iends_with(s1, s2));
}

#ifdef HAS_BOOST

STRING_BENCHMARK(iends_with, boost_cstring)
{
    celero::DoNotOptimizeAway(boost::iends_with(s1.c_str(), s2.c_str()));

    assert(boost::iends_with(s1, s2));
}

STRING_BENCHMARK(iends_with, boost_string)
{
    celero::DoNotOptimizeAway(boost::iends_with(s1, s2));

    assert(boost::iends_with(s1, s2));
}

#endif // HAS_BOOST