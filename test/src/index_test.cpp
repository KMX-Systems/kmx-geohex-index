#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <kmx/geohex/cell/base.hpp>
#include <kmx/geohex/index.hpp>

namespace kmx::geohex
{
    TEST_CASE("index - mode")
    {
        index index {};
        constexpr auto expected_mode = index_mode_t::cell; //_hexagon;
        index.set_mode(expected_mode);
        REQUIRE(index.mode() == expected_mode);
    }

    TEST_CASE("index - resolution")
    {
        index index {};
        constexpr auto expected_resolution = resolution_t::r7;
        index.set_resolution(expected_resolution);
        REQUIRE(index.resolution() == expected_resolution);
    }

    TEST_CASE("index - base cell")
    {
        index index {};
        constexpr auto expected_base_cell = 5u;
        index.set_base_cell(expected_base_cell);
        REQUIRE(index.base_cell() == expected_base_cell);
    }

    TEST_CASE("index - value")
    {
        index index {};
        constexpr auto expected_value = 1234567890u;
        index.set_value(expected_value);
        REQUIRE(index.value() == expected_value);
    }

    TEST_CASE("index - digit")
    {
        index index {};
        for (std::uint8_t i = 0u; i != index.digit_count(); ++i)
        {
            const index::digit_t expectd_digit = (i + 1u) % 8u;
            index.set_digit(i, expectd_digit);
            const auto digit = index.digit(i);
            REQUIRE(digit == expectd_digit);
        }
    }

    TEST_CASE("index - comparison")
    {
        index a {}, b {};
        a = 123;
        b = 1234;
        bool lt = a < b;
        REQUIRE(lt);
        bool gt = a > b;
        REQUIRE(!gt);
        bool eq = a == b;
        REQUIRE(!eq);
        bool neq = a > b;
        REQUIRE(!neq);
    }

    TEST_CASE("index - decoding")
    {
        constexpr index::value_t id {0x85283473fffffffu};
        const index a {id};
        REQUIRE(a.value() == id);
        REQUIRE(a.mode() == index_mode_t::cell); //_hexagon);
        REQUIRE(a.resolution() == resolution_t::r5);
        REQUIRE(a.base_cell() == 20u);
        REQUIRE(a.digit(0u) == 0u);
        REQUIRE(a.digit(1u) == 6u);
        REQUIRE(a.digit(2u) == 4u);
        REQUIRE(a.digit(3u) == 3u);
        REQUIRE(a.digit(4u) == 4u);
        REQUIRE(a.digit(5u) == 7u);
        REQUIRE(a.digit(6u) == 7u);
        REQUIRE(a.digit(7u) == 7u);
        REQUIRE(a.digit(8u) == 7u);
        REQUIRE(a.digit(9u) == 7u);
        REQUIRE(a.digit(10u) == 7u);
        REQUIRE(a.digit(11u) == 7u);
        REQUIRE(a.digit(12u) == 7u);
        REQUIRE(a.digit(13u) == 7u);
        REQUIRE(a.digit(14u) == 7u);

        using number_buffer = std::array<char, 16u>;

        number_buffer buffer {};
        index::number_span no(buffer.begin(), buffer.end());
        a.get_number(no);

        number_buffer expected_no_buffer {"064347777777777"};
        const index::number_span expected_no(expected_no_buffer.begin(), expected_no_buffer.end());

        for (index::digit_index i {}; i != index::digit_count(); ++i)
        {
            REQUIRE(no[i] == expected_no[i]);
        }
    }

    TEST_CASE("index - valid cell base")
    {
        index a {};
        REQUIRE(a.mode() == index_mode_t::invalid);
        REQUIRE(a.resolution() == resolution_t::r0);
        REQUIRE(a.base_cell() == 0);
        for (cell::base::id_t i = 0; i != cell::base::count; ++i)
        {
            a.set_base_cell(i);
            REQUIRE(a.base_cell() == i);
        }
    }
}
