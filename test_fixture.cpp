#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include "fixture.hpp"
#ifdef HAS_BOOST
#include <boost/algorithm/string/predicate.hpp>
#endif

// Should be enough to test char-based strings, innit?
using nfixture = string_benchmark::string_fixture<char>;
using nstring = typename nfixture::string;
using nchar = typename nstring::value_type;

TEST_CASE("starts_with(char* A, char* B)")
{
    nfixture::string a{"something1something2"};
    nfixture::string b{"something1"};

    SECTION("A and B are empty")
    {
        a.clear();
        b.clear();

        SECTION("starts_with(A, B) == true")
        {
            REQUIRE(nfixture::starts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("starts_with(B, A) == true")
        {
            REQUIRE(nfixture::starts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A is empty and B is not empty")
    {
        a.clear();

        SECTION("starts_with(A, B) == false")
        {
            REQUIRE(!nfixture::starts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("starts_with(B, A) == true")
        {
            REQUIRE(nfixture::starts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and A == B")
    {
        b = a;

        SECTION("starts_with(A, B) == true")
        {
            REQUIRE(nfixture::starts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(a.c_str(), b.c_str()));
#endif
        }
        SECTION("starts_with(B, A) == true")
        {
            REQUIRE(nfixture::starts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and starts with B and strlen(A) > strlen(B)")
    {
        SECTION("starts_with(A, B) == true")
        {
            REQUIRE(nfixture::starts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("starts_with(B, A) == false")
        {
            REQUIRE(!nfixture::starts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and not starts with B and strlen(A) > strlen(B)")
    {
        b.assign(b.size(), 'x');
        SECTION("starts_with(A, B) == false")
        {
            REQUIRE(!nfixture::starts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(a.c_str(), b.c_str()));
#endif
        }
    }

    SECTION("A not empty and not starts with B and strlen(B) > strlen(A)")
    {
        b.assign(a.size() * 3, 'x');
        SECTION("starts_with(A, B) == false")
        {
            REQUIRE(!nfixture::starts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(b.c_str(), a.c_str()));
#endif
        }
    }
}

TEST_CASE("ends_with(char* A, char* B)")
{
    nfixture::string a{"something1something2"};
    nfixture::string b{"something2"};

    SECTION("A and B are empty")
    {
        a.clear();
        b.clear();

        SECTION("ends_with(A, B) == true")
        {
            REQUIRE(nfixture::ends_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("ends_with(B, A) == true")
        {
            REQUIRE(nfixture::ends_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A is empty and B is not empty")
    {
        a.clear();

        SECTION("ends_with(A, B) == false")
        {
            REQUIRE(!nfixture::starts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("ends_with(B, A) == true")
        {
            REQUIRE(nfixture::starts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and A == B")
    {
        b = a;

        SECTION("starts_with(A, B) == true")
        {
            REQUIRE(nfixture::ends_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(a.c_str(), b.c_str()));
#endif
        }
        SECTION("starts_with(B, A) == true")
        {
            REQUIRE(nfixture::ends_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and ends with B and strlen(A) > strlen(B)")
    {
        SECTION("ends_with(A, B) == true")
        {
            REQUIRE(nfixture::ends_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("ends_with(B, A) == false")
        {
            REQUIRE(!nfixture::ends_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::ends_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and not ends with B and strlen(A) > strlen(B)")
    {
        b.assign(b.size(), 'x');
        SECTION("ends_with(A, B) == false")
        {
            REQUIRE(!nfixture::ends_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::ends_with(a.c_str(), b.c_str()));
#endif
        }
    }

    SECTION("A not empty and not ends with B and strlen(B) > strlen(A)")
    {
        b.assign(a.size() * 3, 'x');

        SECTION("ends_with(A, B) == false")
        {
            REQUIRE(!nfixture::ends_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::ends_with(b.c_str(), a.c_str()));
#endif
        }
    }

}

TEST_CASE("starts_with(string A, string B)")
{
    nfixture::string a{"something1something2"};
    nfixture::string b{"something1"};

    SECTION("A and B are empty")
    {
        a.clear();
        b.clear();

        SECTION("starts_with(A, B) == true")
        {
            REQUIRE(nfixture::starts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(a, b));
#endif
        }

        SECTION("starts_with(B, A) == true")
        {
            REQUIRE(nfixture::starts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(b, a));
#endif
        }
    }

    SECTION("A is empty and B is not empty")
    {
        a.clear();

        SECTION("starts_with(A, B) == false")
        {
            REQUIRE(!nfixture::starts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(a, b));
#endif
        }

        SECTION("starts_with(B, A) == true")
        {
            REQUIRE(nfixture::starts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(b, a));
#endif
        }
    }

    SECTION("A not empty and A == B")
    {
        b = a;

        SECTION("starts_with(A, B) == true")
        {
            REQUIRE(nfixture::starts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(a, b));
#endif
        }
        SECTION("starts_with(B, A) == true")
        {
            REQUIRE(nfixture::starts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(b, a));
#endif
        }
    }

    SECTION("A not empty and starts with B and strlen(A) > strlen(B)")
    {
        SECTION("starts_with(A, B) == true")
        {
            REQUIRE(nfixture::starts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(a, b));
#endif
        }

        SECTION("starts_with(B, A) == false")
        {
            REQUIRE(!nfixture::starts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(b, a));
#endif
        }
    }

    SECTION("A not empty and not starts with B and strlen(A) > strlen(B)")
    {
        b.assign(b.size(), 'x');
        SECTION("starts_with(A, B) == false")
        {
            REQUIRE(!nfixture::starts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(a, b));
#endif
        }
    }

    SECTION("A not empty and not starts with B and strlen(B) > strlen(A)")
    {
        b.assign(a.size() * 3, 'x');
        SECTION("starts_with(A, B) == false")
        {
            REQUIRE(!nfixture::starts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(b, a));
#endif
        }
    }
}

TEST_CASE("ends_with(string A, string B)")
{
    nfixture::string a{"something1something2"};
    nfixture::string b{"something2"};

    SECTION("A and B are empty")
    {
        a.clear();
        b.clear();

        SECTION("ends_with(A, B) == true")
        {
            REQUIRE(nfixture::ends_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(a, b));
#endif
        }

        SECTION("ends_with(B, A) == true")
        {
            REQUIRE(nfixture::ends_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(b, a));
#endif
        }
    }

    SECTION("A is empty and B is not empty")
    {
        a.clear();

        SECTION("ends_with(A, B) == false")
        {
            REQUIRE(!nfixture::starts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::starts_with(a, b));
#endif
        }

        SECTION("ends_with(B, A) == true")
        {
            REQUIRE(nfixture::starts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::starts_with(b, a));
#endif
        }
    }

    SECTION("A not empty and A == B")
    {
        b = a;

        SECTION("starts_with(A, B) == true")
        {
            REQUIRE(nfixture::ends_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(a, b));
#endif
        }
        SECTION("starts_with(B, A) == true")
        {
            REQUIRE(nfixture::ends_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(b, a));
#endif
        }
    }

    SECTION("A not empty and ends with B and strlen(A) > strlen(B)")
    {
        SECTION("ends_with(A, B) == true")
        {
            REQUIRE(nfixture::ends_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::ends_with(a, b));
#endif
        }

        SECTION("ends_with(B, A) == false")
        {
            REQUIRE(!nfixture::ends_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::ends_with(b, a));
#endif
        }
    }

    SECTION("A not empty and not ends with B and strlen(A) > strlen(B)")
    {
        b.assign(b.size(), 'x');
        SECTION("ends_with(A, B) == false")
        {
            REQUIRE(!nfixture::ends_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::ends_with(a, b));
#endif
        }
    }

    SECTION("A not empty and not ends with B and strlen(B) > strlen(A)")
    {
        b.assign(a.size() * 3, 'x');

        SECTION("ends_with(A, B) == false")
        {
            REQUIRE(!nfixture::ends_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::ends_with(b, a));
#endif
        }
    }

}

TEST_CASE("istarts_with(char* A, char* B)")
{
    nfixture::string a{"something1something2"};
    nfixture::string b{"SOMETHING1"};

    SECTION("A and B are empty")
    {
        a.clear();
        b.clear();

        SECTION("istarts_with(A, B) == true")
        {
            REQUIRE(nfixture::istarts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::istarts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A is empty and B is not empty")
    {
        a.clear();

        SECTION("istarts_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::istarts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and A == B")
    {
        b = a;

        SECTION("istarts_with(A, B) == true")
        {
            REQUIRE(nfixture::istarts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(a.c_str(), b.c_str()));
#endif
        }
        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::istarts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and starts with B and strlen(A) > strlen(B)")
    {
        SECTION("istarts_with(A, B) == true")
        {
            REQUIRE(nfixture::istarts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("istarts_with(B, A) == false")
        {
            REQUIRE(!nfixture::istarts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and not starts with B and strlen(A) > strlen(B)")
    {
        b.assign(b.size(), 'X');
        SECTION("istarts_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(a.c_str(), b.c_str()));
#endif
        }
    }

    SECTION("A not empty and not starts with B and strlen(B) > strlen(A)")
    {
        b.assign(a.size() * 3, 'X');
        SECTION("istarts_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(b.c_str(), a.c_str()));
#endif
        }
    }
}

TEST_CASE("iends_with(char* A, char* B)")
{
    nfixture::string a{"something1something2"};
    nfixture::string b{"SOMETHING2"};

    SECTION("A and B are empty")
    {
        a.clear();
        b.clear();

        SECTION("iends_with(A, B) == true")
        {
            REQUIRE(nfixture::iends_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("iends_with(B, A) == true")
        {
            REQUIRE(nfixture::iends_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A is empty and B is not empty")
    {
        a.clear();

        SECTION("iends_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("iends_with(B, A) == true")
        {
            REQUIRE(nfixture::istarts_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and A == B")
    {
        b = a;

        SECTION("istarts_with(A, B) == true")
        {
            REQUIRE(nfixture::iends_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(a.c_str(), b.c_str()));
#endif
        }
        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::iends_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and ends with B and strlen(A) > strlen(B)")
    {
        SECTION("iends_with(A, B) == true")
        {
            REQUIRE(nfixture::iends_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(a.c_str(), b.c_str()));
#endif
        }

        SECTION("iends_with(B, A) == false")
        {
            REQUIRE(!nfixture::iends_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::iends_with(b.c_str(), a.c_str()));
#endif
        }
    }

    SECTION("A not empty and not ends with B and strlen(A) > strlen(B)")
    {
        b.assign(b.size(), 'X');
        SECTION("iends_with(A, B) == false")
        {
            REQUIRE(!nfixture::iends_with(a.c_str(), b.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::iends_with(a.c_str(), b.c_str()));
#endif
        }
    }

    SECTION("A not empty and not ends with B and strlen(B) > strlen(A)")
    {
        b.assign(a.size() * 3, 'X');

        SECTION("iends_with(A, B) == false")
        {
            REQUIRE(!nfixture::iends_with(b.c_str(), a.c_str()));
#ifdef HAS_BOOST
            REQUIRE(!boost::iends_with(b.c_str(), a.c_str()));
#endif
        }
    }

}

TEST_CASE("istarts_with(string A, string B)")
{
    nfixture::string a{"something1something2"};
    nfixture::string b{"SOMETHING1"};

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

    SECTION("A not empty and starts with B and strlen(A) > strlen(B)")
    {
        SECTION("istarts_with(A, B) == true")
        {
            REQUIRE(nfixture::istarts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::istarts_with(a, b));
#endif
        }

        SECTION("istarts_with(B, A) == false")
        {
            REQUIRE(!nfixture::istarts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(b, a));
#endif
        }
    }

    SECTION("A not empty and not starts with B and strlen(A) > strlen(B)")
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

    SECTION("A not empty and not starts with B and strlen(B) > strlen(A)")
    {
        b.assign(a.size() * 3, 'X');
        SECTION("istarts_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(b, a));
#endif
        }
    }
}

TEST_CASE("iends_with(string A, string B)")
{
    nfixture::string a{"something1something2"};
    nfixture::string b{"SOMETHING2"};

    SECTION("A and B are empty")
    {
        a.clear();
        b.clear();

        SECTION("iends_with(A, B) == true")
        {
            REQUIRE(nfixture::iends_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(a, b));
#endif
        }

        SECTION("iends_with(B, A) == true")
        {
            REQUIRE(nfixture::iends_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(b, a));
#endif
        }
    }

    SECTION("A is empty and B is not empty")
    {
        a.clear();

        SECTION("iends_with(A, B) == false")
        {
            REQUIRE(!nfixture::istarts_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::istarts_with(a, b));
#endif
        }

        SECTION("iends_with(B, A) == true")
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
            REQUIRE(nfixture::iends_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(a, b));
#endif
        }
        SECTION("istarts_with(B, A) == true")
        {
            REQUIRE(nfixture::iends_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(b, a));
#endif
        }
    }

    SECTION("A not empty and ends with B and strlen(A) > strlen(B)")
    {
        SECTION("iends_with(A, B) == true")
        {
            REQUIRE(nfixture::iends_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(boost::iends_with(a, b));
#endif
        }

        SECTION("iends_with(B, A) == false")
        {
            REQUIRE(!nfixture::iends_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::iends_with(b, a));
#endif
        }
    }

    SECTION("A not empty and not ends with B and strlen(A) > strlen(B)")
    {
        b.assign(b.size(), 'X');
        SECTION("iends_with(A, B) == false")
        {
            REQUIRE(!nfixture::iends_with(a, b));
#ifdef HAS_BOOST
            REQUIRE(!boost::iends_with(a, b));
#endif
        }
    }

    SECTION("A not empty and not ends with B and strlen(B) > strlen(A)")
    {
        b.assign(a.size() * 3, 'X');

        SECTION("iends_with(A, B) == false")
        {
            REQUIRE(!nfixture::iends_with(b, a));
#ifdef HAS_BOOST
            REQUIRE(!boost::iends_with(b, a));
#endif
        }
    }

}
