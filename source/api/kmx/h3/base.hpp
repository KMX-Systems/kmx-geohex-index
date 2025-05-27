/// @file h3/base.hpp
#pragma once
#ifndef PCH
    #include <cstdint>
    #include <utility>
#endif

namespace kmx::h3
{
    template <typename T>
    constexpr auto operator+(const T e) noexcept
    {
        return static_cast<std::underlying_type_t<T>>(e);
    }

    namespace coordinate
    {
        class ij;
        class ijk;
        class wgs84;
    }

    enum class index_mode_t : std::uint8_t
    {
        invalid,
        cell,
        edge_unidirectional,
        edge_bidirectional,
        vertex,
    };

    constexpr auto index_mode_count = +index_mode_t::vertex + 1u;

    enum class resolution_t : std::uint8_t
    {
        r0, // lowest detail
        r1,
        r2,
        r3,
        r4,
        r5,
        r6,
        r7,
        r8,
        r9,
        r10,
        r11,
        r12,
        r13,
        r14,
        r15, // highest detail
    };

    constexpr auto resolution_count = +resolution_t::r15 + 1u;

    enum class direction_t : std::uint8_t
    {
        center,  // center
        k_axes,  // k-axes direction
        j_axes,  // j-axes direction
        jk_axes, // j == k direction (j_axes | k_axes_digit)
        i_axes,  // i-axes direction
        ik_axes, // i == k direction (i_axes_digit | k_axes_digit)
        ij_axes, // i == j direction (i_axes_digit | j_axes_digit)
        invalid, // invalid direction
    };

    constexpr auto direction_count = +direction_t::invalid + 1u;

    direction_t rotate_60ccw(const direction_t digit) noexcept;
    direction_t rotate_60cw(const direction_t digit) noexcept;

    using k_distance = std::uint32_t;

    namespace cell::base
    {
        using id_t = std::uint8_t;
    }

    enum class error_t : std::uint8_t
    {
        none,
    };

    class index;
}
