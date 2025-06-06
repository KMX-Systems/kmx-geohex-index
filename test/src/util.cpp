/// @file geohex/util.cpp
#include <catch2/catch_all.hpp>
#include <kmx/geohex/util.hpp>

namespace kmx::geohex
{
    TEST_CASE("utils - radian to degree conversion")
    {
        const auto expected_degrees = 30.0;
        const auto radians = degree::to_radian(expected_degrees);
        const auto resulted_degrees = std::round(radian::to_degree(radians));
        REQUIRE(resulted_degrees == expected_degrees);
    }
}
