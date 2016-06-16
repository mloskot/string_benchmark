#pragma once
#include <hayai.hpp>
#include "benchmark.hpp"
#include "fixture.hpp"

template <typename Char>
struct test_append
{
    using f = fixture<Char>;

    static void append(Char const* const s)
    {
        f::string result;
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
        f::string result;

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
