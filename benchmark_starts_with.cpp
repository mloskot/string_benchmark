#define CELERO_STATIC
#include <algorithm>
#include <iostream>
#ifdef HAS_BOOST
#include <boost/algorithm/string/predicate.hpp>
#endif
#include "benchmark.hpp"
#include "fixture.hpp"

CELERO_MAIN

STRING_BASELINE(starts_with, strncmp)
{
    celero::DoNotOptimizeAway(fixture::strncmp(s1.c_str(), s2.c_str(), s1.size()));

    assert(s1.size() == s1.size());
    assert(fixture::strncmp(s1.c_str(), s2.c_str(), s1.size()) == 0);
}

STRING_BENCHMARK(starts_with, strnicmp_cstr)
{
    celero::DoNotOptimizeAway(fixture::starts_with(s1.c_str(), s2.c_str()));

    assert(fixture::starts_with(s1.c_str(), s2.c_str()));
}

STRING_BENCHMARK(starts_with, strnicmp_stdstr)
{
    celero::DoNotOptimizeAway(fixture::starts_with(s1, s2));

    assert(fixture::starts_with(s1, s2));
}

#ifdef HAS_BOOST

STRING_BENCHMARK(starts_with, boost_cstr)
{
    celero::DoNotOptimizeAway(boost::starts_with(s1.c_str(), s2.c_str()));

    assert(boost::starts_with(s1, s2));
}

STRING_BENCHMARK(starts_with, boost_stdstr)
{
    celero::DoNotOptimizeAway(boost::starts_with(s1, s2));

    assert(boost::starts_with(s1, s2));
}

#endif // HAS_BOOST
