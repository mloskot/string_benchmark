#define CELERO_STATIC
#include <algorithm>
#include <iostream>
#include "benchmark.hpp"
#include "fixture.hpp"
#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include "fmt/format.h"

namespace
{

template <typename CharT>
struct attached_string_buffer final : public fmt::Buffer<CharT>
{
    using base_buffer = fmt::Buffer<CharT>;
    std::basic_string<CharT>& m_str; // of size() of number of characters plus the null terminator (sprintf-like)

    attached_string_buffer(std::basic_string<CharT>& str) : m_str{str}
    {
        // std::string is used as array, with size() of currently writable memory (== fmt::Buffer capacity()).
        // and capacity() as how much writable space can grow without reallocation (a buffer overrun).
        // fmt::Buffer::size() is current number characters pushed to buffer.
        assert(m_str.capacity() > 0);
        if (m_str.capacity() == 0)
            throw std::length_error("zero-length buffer is invalid");

        base_buffer::ptr_ = &m_str[0];
        base_buffer::capacity_ = m_str.size();
    }

    void grow(std::size_t capacity) final
    {
        if (capacity > m_str.capacity())
            throw std::length_error("buffer overrun");

        if (capacity > m_str.size())
        {
            capacity = std::min(m_str.capacity(), static_cast<std::size_t>(fmt::internal::INLINE_BUFFER_SIZE));
            capacity = std::min(capacity + capacity / 2, m_str.capacity());
            m_str.resize(capacity); // capacity < m_str.capacity(), only moves EOS without reallocation
            base_buffer::capacity_ = m_str.size();
            assert(base_buffer::ptr_ == &m_str[0]);
        }
    }
};

template <typename CharT>
struct attached_string_writer : public fmt::BasicWriter<CharT>
{
    attached_string_buffer<CharT> m_buffer;
    attached_string_writer(std::basic_string<CharT>& str) : fmt::BasicWriter<CharT>(m_buffer), m_buffer(str) {}
};

template<typename CharT, typename... ARGS>
inline void static_format(std::basic_string<CharT>& strResult, CharT const* szFmt, ARGS const&... args)
{
    attached_string_writer<CharT> writer(strResult);
    writer.write(szFmt, args...);
    assert(writer.size() <= strResult.size());
    strResult.resize(writer.size()); // only moves EOS, no memory shrink
}

// fmtlib sets default buffer size to 500
auto constexpr default_formatter = STRING_TEXT("abc def ghi jkl mno {:.7f} abc def ghi jkl mno {} abc def ghi jkl mno {} abc def ghi jkl mno");

} // unnamed namespace

STRING_BASELINE(fmtlib, default_buffer)
{
    auto result = fmt::format(default_formatter, 1209.123456, 1234567890, STRING_TEXT("ABC DEF GHI JKL MNO"));
    celero::DoNotOptimizeAway(result);

    assert(result.size() >= fixture::strlen(default_formatter));
}

STRING_BENCHMARK(fmtlib, string_buf32)
{
#ifdef _DEBUG
    void const* p = buffer32_.data();
#endif

    static_format(buffer32_, STRING_TEXT("{:.2f} {} {}"), 1209.123456, 1234567890, STRING_TEXT("abc"));
    celero::DoNotOptimizeAway(buffer32_);

    assert(buffer32_.size() <= 32);
    assert(buffer32_.data() == p);
}

STRING_BENCHMARK(fmtlib, string_buf256)
{
#ifdef _DEBUG
    void const* p = buffer256_.data();
#endif

    static_format(buffer256_, STRING_TEXT("{:.5f} abcdef {} ABCDEF {} abc def ghi"), 1209.123456, 1234567890, STRING_TEXT("abc"));
    celero::DoNotOptimizeAway(buffer256_);

    assert(buffer256_.size() <= 256);
    assert(buffer256_.data() == p);
}

STRING_BENCHMARK(fmtlib, string_buf512)
{
#ifdef _DEBUG
    void const* p = buffer512_.data();
#endif

    static_format(buffer512_, STRING_TEXT("abc def ghi jkl mno {:.7f} abc def ghi jkl mno {} abc def ghi jkl mno {} abc def ghi jkl mno"), 1209.123456, 1234567890, STRING_TEXT("ABC DEF GHI JKL MNO"));
    celero::DoNotOptimizeAway(buffer512_);

    assert(buffer512_.size() <= 512);
    assert(buffer512_.data() == p);
}

STRING_BENCHMARK(fmtlib, string_buf1024)
{
#ifdef _DEBUG
    void const* p = buffer1024_.data();
#endif

    static_format(buffer1024_, STRING_TEXT("abc def ghi jkl mno {:.7f} abc def ghi jkl mno {} abc def ghi jkl mno {} abc def ghi jkl mno"), 1209.123456, 1234567890, STRING_TEXT("ABC DEF GHI JKL MNO"));
    celero::DoNotOptimizeAway(buffer1024_);

    assert(buffer1024_.size() <= 1024);
    assert(buffer1024_.data() == p);
}

CELERO_MAIN
