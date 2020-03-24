#define CELERO_STATIC
#include <random>
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
    std::vector<point_t> points_;

    std::vector<celero::TestFixture::ExperimentValue> getExperimentValues() const override
    {
        // Problem space as number of points (pairs of X/Y)
        std::vector<celero::TestFixture::ExperimentValue> v;
        v.emplace_back(1, 0);
        v.emplace_back(256, 0);
        v.emplace_back(512, 0);
        v.emplace_back(1024, 0);
        return v;
    }

    void setUp(celero::TestFixture::ExperimentValue const& e) override
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_real_distribution<> d(-180.0, 180.0);

        auto const size = static_cast<std::size_t>(e.Value);
        this->points_.reserve(size);
        for (std::size_t i = 0; i < size; i++)
        {
            auto const x = d(g);
            auto const y = d(g);

            boost::geometry::append(this->multi_point_, boost::geometry::make<point_t>(x, y));
            this->points_.emplace_back(x, y);
        }
    }

    void tearDown() override
    {
        multi_point_ = {};
        points_ = {};
    }
};

CELERO_MAIN

BASELINE_F(wkt, to_string, Fixture, 0, 100)
{
    for (auto const& p : this->points_)
    {
        celero::DoNotOptimizeAway(std::to_string(p.x()));
        celero::DoNotOptimizeAway(std::to_string(p.y()));
    }
}

BENCHMARK_F(wkt, stringstream, Fixture, 0, 100)
{
    std::ostringstream oss;
    for (auto const& p : this->points_)
    {
        celero::DoNotOptimizeAway(oss << p.x());
        celero::DoNotOptimizeAway(oss << p.y());
    }
}

BENCHMARK_F(wkt, lexical_cast, Fixture, 0, 100)
{
    std::ostringstream oss;
    for (auto const& p : this->points_)
    {
        celero::DoNotOptimizeAway(boost::lexical_cast<std::string>(p.x()));
        celero::DoNotOptimizeAway(boost::lexical_cast<std::string>(p.y()));
    }
}

BENCHMARK_F(wkt, wkt, Fixture, 0, 100)
{
    std::ostringstream oss;
    celero::DoNotOptimizeAway(oss << boost::geometry::wkt(this->multi_point_));
    celero::DoNotOptimizeAway(oss.str());
}

#ifdef BOOST_GEOMETRY_PR670
BENCHMARK_F(wkt, to_wkt, Fixture, 0, 100)
{
    celero::DoNotOptimizeAway(boost::geometry::to_wkt(this->multi_point_));
}

BENCHMARK_F(wkt, to_wkt_6digits, Fixture, 0, 100)
{
    celero::DoNotOptimizeAway(boost::geometry::to_wkt(this->multi_point_));
}
#endif
