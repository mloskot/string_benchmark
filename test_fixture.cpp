#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include "fixture.hpp"
#ifdef HAS_BOOST
#include <boost/algorithm/string/predicate.hpp>
#endif

using nfixture = string_benchmark::string_fixture<char>;
using wfixture = string_benchmark::string_fixture<wchar_t>;

TEST_CASE("istarts_with(A, B)")
{
    nfixture::string a{"SOMETHING1"};
    nfixture::string b{"something1something2"};

    SECTION("A and B are empty")
    {
        a.clear();
        b.clear();

        SECTION("istarts_with(A, B) == true")
        {
            REQUIRE(nfixture::istarts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(a, b));
#endif
        }

        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::istarts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(b, a));
#endif
        }
    }

    SECTION("A is empty and B is not empty")
    {
        a.clear();

        SECTION("istarts_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(a, b));
#endif
        }

        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::istarts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(b, a));
#endif
        }
    }

    SECTION("A not empty and A == B")
    {
        b = a;

        SECTION("istarts_with(A, B) == true")
        {
            REQUIRE(nfixture::istarts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(a, b));
#endif
        }
        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::istarts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(b, a));
#endif
        }
    }

    SECTION("A not empty and starts with B and strlen(A) < strlen(B)")
    {
        SECTION("istarts_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(a, b));
#endif
        }
    }

    SECTION("A not empty and not starts with B and strlen(A) < strlen(B)")
    {
        b.assign(b.size(), 'X');
        SECTION("istarts_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(a, b));
#endif
        }
    }

    SECTION("B not empty and starts with A and strlen(B) > strlen(A)")
    {
        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::istarts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(b, a));
#endif
        }
    }

    SECTION("B not empty and not starts with A and strlen(B) > strlen(A)")
    {
        a.assign(a.size(), 'X');
        SECTION("istarts_with(B, A) == false")
        {
            REQUIRE(!nfixture::istarts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(b, a));
#endif
        }
    }
}
