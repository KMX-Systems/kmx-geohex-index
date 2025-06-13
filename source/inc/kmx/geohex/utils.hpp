/// @file inc/kmx/geohex/utils.hpp
/// @ingroup Internal
/// @brief Core enumerations, type aliases, and fundamental utilities for the GeoHex library.
#pragma once
#ifndef PCH
    #include <cmath>
    #include <kmx/geohex/base.hpp>
    #include <numbers>
#endif

namespace kmx::geohex
{
    constexpr double sqrt3_2 = 0.8660254037844386467637231707529361834714;

    using pseudo_ijk = std::tuple<std::int8_t, std::int8_t, std::int8_t>;

    /// @brief Converts an angle from degrees to radians.
    /// @param degrees The angle in degrees.
    /// @return The equivalent angle in radians.
    constexpr double degrees_to_radians(double degrees) noexcept
    {
        constexpr double rads_per_degree = std::numbers::pi_v<double> / 180.0;
        return degrees * rads_per_degree;
    }

    /// @brief Converts an angle from radians to degrees.
    /// @param radians The angle in radians.
    /// @return The equivalent angle in degrees.
    constexpr double radians_to_degrees(double radians) noexcept
    {
        constexpr double degrees_per_rad = 180.0 / std::numbers::pi_v<double>;
        return radians * degrees_per_rad;
    }

    /// @brief Rotates a direction 60 degrees counter-clockwise.
    /// @param digit The initial direction.
    /// @return The rotated direction.
    direction_t rotate_60ccw(const direction_t digit) noexcept;

    /// @brief Rotates a direction 60 degrees clockwise.
    /// @param digit The initial direction.
    /// @return The rotated direction.
    direction_t rotate_60cw(const direction_t digit) noexcept;

    double scaling_factor(const resolution_t resolution) noexcept;

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
}
