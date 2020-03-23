#define CELERO_STATIC
#include <cstdlib>
#include <string>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/lexical_cast.hpp>
#include <celero/Celero.h>
#include "benchmark.hpp"

// Uncomment to enable boost::geometry::to_wkt from https://github.com/boostorg/geometry/pull/670
#define BOOST_GEOMETRY_PR670

struct Fixture : celero::TestFixture
{
    using point_t = boost::geometry::model::d2::point_xy<double>;
    boost::geometry::model::multi_point<point_t> multi_point_;
    std::vector<double> coordinates_;

    std::vector<celero::TestFixture::ExperimentValue> getExperimentValues() const override
    {
        std::vector<celero::TestFixture::ExperimentValue> v;
        v.emplace_back(1, 0);
        v.emplace_back(256, 0);
        v.emplace_back(512, 0);
        v.emplace_back(1024, 0);
        return v;
    }

    void setUp(celero::TestFixture::ExperimentValue const& e) override
    {
        auto const size = static_cast<std::size_t>(e.Value);
        this->coordinates_.reserve(size);
        for (std::size_t i = 0; i < size; i++)
        {
            srand(unsigned(i * time(NULL)));
            boost::geometry::append(this->multi_point_,
                boost::geometry::make<point_t>(
                    (double)std::rand() / (RAND_MAX),
                    (double)std::rand() / (RAND_MAX)));
            this->coordinates_.push_back((double)std::rand() / (RAND_MAX));
        }
    }

    void tearDown() override
    {
        multi_point_ = {};
        coordinates_ = {};
    }
};

CELERO_MAIN

BASELINE_F(wkt, to_string, Fixture, (string_benchmark::config::samples), (string_benchmark::config::iterations))
{
    for (auto const& v : this->coordinates_)
        celero::DoNotOptimizeAway(std::to_string(v));
}

BENCHMARK_F(wkt, stringstream, Fixture, (string_benchmark::config::samples), (string_benchmark::config::iterations))
{
    std::ostringstream oss;
    for (auto const& v : this->coordinates_)
        celero::DoNotOptimizeAway(oss << v);
}

BENCHMARK_F(wkt, lexical_cast, Fixture, (string_benchmark::config::samples), (string_benchmark::config::iterations))
{
    std::ostringstream oss;
    for (auto const& v : this->coordinates_)
        celero::DoNotOptimizeAway(boost::lexical_cast<std::string>(v));
}

BENCHMARK_F(wkt, geometry_wkt, Fixture, (string_benchmark::config::samples), (string_benchmark::config::iterations))
{
    std::ostringstream oss;
    celero::DoNotOptimizeAway(oss << boost::geometry::wkt(this->multi_point_));
}

#ifdef BOOST_GEOMETRY_PR670
BENCHMARK_F(wkt, geometry_to_wkt, Fixture, (string_benchmark::config::samples), (string_benchmark::config::iterations))
{
    celero::DoNotOptimizeAway(boost::geometry::to_wkt(this->multi_point_));
}
#endif
