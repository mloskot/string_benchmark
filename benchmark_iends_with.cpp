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
    celero::DoNotOptimizeAway(fixture::strnicmp(si1.c_str(), si2.c_str(), si1.size()));

    assert(si1.size() == si1.size());
    assert(fixture::strnicmp(si1.c_str(), si2.c_str(), si1.size()) == 0);
}

STRING_BENCHMARK(iends_with, strnicmp_cstr)
{
    celero::DoNotOptimizeAway(fixture::iends_with(si1.c_str(), si2.c_str()));

    assert(fixture::iends_with(si1.c_str(), si2.c_str()));
}

STRING_BENCHMARK(iends_with, strnicmp_stdstr)
{
    celero::DoNotOptimizeAway(fixture::iends_with(si1, si2));

    assert(fixture::iends_with(si1, si2));
}

#ifdef HAS_BOOST

STRING_BENCHMARK(iends_with, boost_cstr)
{
    celero::DoNotOptimizeAway(boost::iends_with(si1.c_str(), si2.c_str()));

    assert(boost::iends_with(si1, si2));
}

STRING_BENCHMARK(iends_with, boost_stdstr)
{
    celero::DoNotOptimizeAway(boost::iends_with(si1, si2));

    assert(boost::iends_with(si1, si2));
}

#endif // HAS_BOOST
