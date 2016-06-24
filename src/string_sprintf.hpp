#pragma once
#include <hayai.hpp>
#include "benchmark.hpp"
#include "fixture.hpp"
#include <iostream>

template <typename Char>
struct test_sprintf
{
    using f = fixture<Char>;
    using string = typename fixture<Char>::string;

    static void sprintf(Char const* const s)
    {
        string result;
        result = f::sprintf(f::formatter_s(), s);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        auto const new_size = result.size();
        assert(new_size == f::strlen(s));
#endif
    }

    static void single_chars10()
    {
        auto const& s = f::samples().front();
        sprintf(s);
    }

    static void single_chars100()
    {
        auto const& s = f::samples().back();
        sprintf(s);
    }

    static void multiple()
    {
        string result;

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        std::size_t test_size = result.size();
#endif

        auto const& samples = f::samples();
        auto const& s1 = samples[0];
        auto const& s2 = samples[1];
        auto const& s3 = samples[2];
        auto const& s4 = samples[3];
        auto const& s5 = samples[4];
        auto const& s6 = samples[5];
        auto const& s7 = samples[6];
        auto const& s8 = samples[7];
        auto const& s9 = samples[8];
        auto const& s10 = samples[9];

        result = f::sprintf(f::formatter_s10(), s1, s2, s3, s4, s5, s6, s7, s8, s9, s10);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        for (auto const& s : samples)
            test_size += f::strlen(s) + 1;
        assert(result.size() == test_size);
#endif
    }
};

STRING_BENCHMARK(string, sprintf_single_chars10)
{
    test_sprintf<char>::single_chars10();
}

STRING_BENCHMARK(string, sprintf_single_chars100)
{
    test_sprintf<char>::single_chars100();
}

STRING_BENCHMARK(string, sprintf_multiple)
{
    test_sprintf<char>::multiple();
}

STRING_BENCHMARK(wstring, sprintf_single_chars10)
{
    test_sprintf<wchar_t>::single_chars10();
}

STRING_BENCHMARK(wstring, sprintf_single_chars100)
{
    test_sprintf<wchar_t>::single_chars100();
}

STRING_BENCHMARK(wstring, sprintf_multiple)
{
    test_sprintf<wchar_t>::multiple();
}

STRING_BENCHMARK(string, sprintf_multiple_sql_example)
{
    auto const* const tb = "this_is_table_name";
    auto const* const c1 = "col1_name";
    auto const* const t1 = "INT";
    auto const* const c2 = "col2_name";
    auto const* const t2 = "VARCHAR";
    auto const s2 = 120U;

    std::string r;
    r = fixture<char>::sprintf("CREATE TABLE %s (%s %s, %s %s(%u));",
        tb, c1, t1, c2, t2, s2);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r.size() == 72);
#endif
}

STRING_BENCHMARK(wstring, sprintf_multiple_sql_example)
{
    auto const* const tb = L"this_is_table_name";
    auto const* const c1 = L"col1_name";
    auto const* const t1 = L"INT";
    auto const* const c2 = L"col2_name";
    auto const* const t2 = L"VARCHAR";
    auto const s2 = 120U;

    std::wstring r;
    r = fixture<wchar_t>::sprintf(L"CREATE TABLE %s (%s %s, %s %s(%u));",
        tb, c1, t1, c2, t2, s2);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r.size() == 72);
#endif
}
