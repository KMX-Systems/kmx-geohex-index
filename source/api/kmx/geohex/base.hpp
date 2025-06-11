/// @file geohex/base.hpp
#pragma once
#ifndef PCH
    #include <cmath>
    #include <cstdint>
    #include <functional>
    #include <numbers>
#endif

namespace kmx::gis::wgs84
{
    class coordinate;
}

namespace kmx
{
    constexpr double sqrt3_2 = 0.8660254037844386467637231707529361834714;

    template <typename T>
        requires std::is_enum_v<T>
    constexpr auto operator+(const T e) noexcept
    {
        return static_cast<std::underlying_type_t<T>>(e);
    }

    template <class T>
    void hash_combine(std::size_t& seed, const T& v) noexcept
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9u + (seed << 6u) + (seed >> 2u);
    }
}

namespace kmx::geohex
{
    namespace coordinate
    {
        class ij;
        class ijk;
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

    /// @ref _isResolutionClassIII (H3 C internal)
    /// @brief Determines if a resolution is Class III (odd).
    /// @param res The resolution.
    /// @return True if Class III, false otherwise (Class II - even).
    constexpr bool is_class_3(const resolution_t res) noexcept
    {
        return (+res % 2u) != 0u;
    }

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

    constexpr auto direction_count = +direction_t::invalid;

    direction_t rotate_60ccw(const direction_t digit) noexcept;
    direction_t rotate_60cw(const direction_t digit) noexcept;

    using k_distance = std::uint32_t;

    namespace cell::base
    {
        using id_t = std::uint8_t;
    }

    enum class error_t : std::uint8_t
    {
        none = 0,
        failed,
        domain,
        latlng_domain,
        res_domain,
        cell_invalid,
        dir_edge_invalid,
        undir_edge_invalid,
        vertex_invalid,
        pentagon,
        duplicate_input,
        not_neighbors,
        res_mismatch,
        memory_alloc,
        memory_bounds,
        option_invalid,
    };

    class index;

    template <typename _Float = double, typename _Int = int>
    constexpr void cube_round(const _Float i, const _Float j, const _Float k, _Int& i_out, _Int& j_out, _Int& k_out) noexcept
    {
        _Int ri = std::round(i);
        _Int rj = std::round(j);
        _Int rk = std::round(k);

        const auto i_diff = std::abs(static_cast<_Float>(ri) - i);
        const auto j_diff = std::abs(static_cast<_Float>(rj) - j);
        const auto k_diff = std::abs(static_cast<_Float>(rk) - k);

        if ((i_diff > j_diff) && (i_diff > k_diff)) // round, maintaining valid cube coords.
            ri = -rj - rk;
        else if (j_diff > k_diff)
            rj = -ri - rk;
        else
            rk = -ri - rj;

        i_out = ri;
        j_out = rj;
        k_out = rk;
    }

    double scaling_factor(const resolution_t resolution);

    /// @brief Converts degrees to radians.
    /// @param degrees Angle in degrees.
    /// @return Angle in radians.
    constexpr double degrees_to_radians(double degrees) noexcept
    {
        constexpr double rads_per_degree = std::numbers::pi_v<double> / 180.0;
        return degrees * rads_per_degree;
    }

    /// @brief Converts radians to degrees.
    /// @param radians Angle in radians.
    /// @return Angle in degrees.
    constexpr double radians_to_degrees(double radians) noexcept
    {
        constexpr double degrees_per_rad = 180.0 / std::numbers::pi_v<double>;
        return radians * degrees_per_rad;
    }
}
