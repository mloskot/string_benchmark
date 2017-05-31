#define CELERO_STATIC
#include <algorithm>
#include <iostream>
#ifdef HAS_BOOST
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#endif
#include "benchmark.hpp"
#include "fixture.hpp"

CELERO_MAIN

STRING_BASELINE_SI(escape_xml, single_pass, 0, 100)
{
    auto out = fixture::escape_xml(sxml);
    celero::DoNotOptimizeAway(out);
}

STRING_BENCHMARK_SI(escape_xml, multi_pass, 0, 100)
{
    using xml = string_benchmark::xml<STRING_CHAR_TYPE>;
    fixture::replace(sxml, xml::amp, xml::entity_amp);
    fixture::replace(sxml, xml::apos, xml::entity_apos);
    fixture::replace(sxml, xml::lt, xml::entity_lt);
    fixture::replace(sxml, xml::gt, xml::entity_gt);
    fixture::replace(sxml, xml::quot, xml::entity_quot);
}

#ifdef HAS_BOOST

STRING_BENCHMARK_SI(escape_xml, boost_multi_pass, 0, 100)
{
    using xml = string_benchmark::xml<STRING_CHAR_TYPE>;
    boost::algorithm::replace_all(sxml, xml::amp, xml::entity_amp);
    boost::algorithm::replace_all(sxml, xml::apos, xml::entity_apos);
    boost::algorithm::replace_all(sxml, xml::lt, xml::entity_lt);
    boost::algorithm::replace_all(sxml, xml::gt, xml::entity_gt);
    boost::algorithm::replace_all(sxml, xml::quot, xml::entity_quot);
}

#endif // HAS_BOOST
