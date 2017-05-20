#pragma once
#include <hayai.hpp>
#include "benchmark.hpp"
#include "fixture.hpp"

template <typename Char>
struct test_append
{
    using f = fixture<Char>;
    using string = typename fixture<Char>::string;

    static void append(Char const* const s)
    {
        string result;
        result.append(s);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        auto const new_size = result.size();
        assert(new_size == f::strlen(s));
#endif
    }

    static void single_chars10()
    {
        auto const& s = f::samples().front();
        append(s);
    }

    static void single_chars100()
    {
        auto const& s = f::samples().back();
        append(s);
    }

    static void multiple()
    {
        string result;

#ifdef STRING_BENCHMARK_ENABLE_TESTS
        std::size_t test_size = result.size();
#endif
        auto const& samples = f::samples();
        for (auto const& s : samples)
        {
            result.append(s);

#ifdef STRING_BENCHMARK_ENABLE_TESTS
            auto const new_size = result.size();
            assert(new_size == test_size + f::strlen(s));
            test_size = new_size;
#endif
        }
    }
};

STRING_BENCHMARK(string, append_single_chars10)
{
    test_append<char>::single_chars10();
}

STRING_BENCHMARK(string, append_single_chars100)
{
    test_append<char>::single_chars100();
}

STRING_BENCHMARK(string, append_multiple)
{
    test_append<char>::multiple();
}

STRING_BENCHMARK(wstring, append_single_chars10)
{
    test_append<wchar_t>::single_chars10();
}

STRING_BENCHMARK(wstring, append_single_chars100)
{
    test_append<wchar_t>::single_chars10();
}

STRING_BENCHMARK(wstring, append_multiple)
{
    test_append<wchar_t>::multiple();
}

STRING_BENCHMARK(string, append_multiple_sql_example)
{
    auto const* const tb = "this_is_table_name";
    auto const* const c1 = "col1_name";
    auto const* const t1 = "INT";
    auto const* const c2 = "col2_name";
    auto const* const t2 = "VARCHAR";
    auto const s2 = 120U;

    std::string r("CREATE TABLE ");
    r += tb;
    r += " (";
    r += c1;
    r += ' ';
    r += t1;
    r += ", ";
    r += c2;
    r += ' ';
    r += t2;
    r += '(';
    r += std::to_string(s2);
    r += "));";

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r.size() == 72);
#endif
}

STRING_BENCHMARK(wstring, append_multiple_sql_example)
{
    auto const* const tb = L"this_is_table_name";
    auto const* const c1 = L"col1_name";
    auto const* const t1 = L"INT";
    auto const* const c2 = L"col2_name";
    auto const* const t2 = L"VARCHAR";
    auto const s2 = 120U;

    std::wstring r(L"CREATE TABLE ");
    r += tb;
    r += L" (";
    r += c1;
    r += L' ';
    r += t1;
    r += L", ";
    r += c2;
    r += L' ';
    r += t2;
    r += L'(';
    r += std::to_wstring(s2);
    r += L"));";

#ifdef STRING_BENCHMARK_ENABLE_TESTS
    assert(r.size() == 72);
#endif
}
