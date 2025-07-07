/// @file inc/kmx/geohex/utils.hpp
/// @ingroup Internal
/// @brief Provides core mathematical constants, conversion utilities, and helper functions.
#pragma once
#ifndef PCH
    #include <cmath>
    #include <kmx/geohex/base.hpp>
    #include <numbers>
#endif

namespace kmx::math
{
    /// @brief Converts an angle from degrees to radians.
    /// @param degrees The angle in degrees.
    /// @return The equivalent angle in radians.
    constexpr double degrees_to_radians(const double degrees) noexcept
    {
        constexpr double rads_per_degree = std::numbers::pi_v<double> / 180.0;
        return degrees * rads_per_degree;
    }

    /// @brief Converts an angle from radians to degrees.
    /// @param radians The angle in radians.
    /// @return The equivalent angle in degrees.
    constexpr double radians_to_degrees(const double radians) noexcept
    {
        constexpr double degrees_per_rad = 180.0 / std::numbers::pi_v<double>;
        return radians * degrees_per_rad;
    }
}

namespace kmx::geohex
{
    /// @brief A pre-calculated constant for the square root of 3 divided by 2.
    /// @details This value (sin(60°)) is used frequently in hexagonal grid math to convert
    ///          axial/cube coordinates to Cartesian coordinates.
    constexpr double sqrt3_2 = 0.8660254037844386467637231707529361834714;

    /// @brief Rotates a direction 60 degrees counter-clockwise.
    /// @details This is a simple rotational transform on the 7 directions.
    ///          The `CENTER` direction remains unchanged.
    /// @param digit The initial direction.
    /// @return The rotated direction.
    direction_t rotate_60ccw(const direction_t digit) noexcept;

    /// @brief Rotates a direction 60 degrees clockwise.
    /// @details This is a simple rotational transform on the 7 directions.
    ///          The `CENTER` direction remains unchanged.
    /// @param digit The initial direction.
    /// @return The rotated direction.
    direction_t rotate_60cw(const direction_t digit) noexcept;

    /// @brief Retrieves the scaling factor for a given resolution.
    /// @details This factor is used to scale normalized IJK coordinates to the correct
    ///          magnitude for a specific resolution's gnomonic projection plane.
    /// @ref H3's `UNIT_VEC_SCALES` constant array.
    /// @param resolution The resolution.
    /// @return The resolution-specific scaling factor.
    double scaling_factor(const resolution_t resolution) noexcept;

    /// @brief Rounds floating-point cube coordinates to the nearest integer cube coordinate.
    /// @details This is a fundamental algorithm in for converting from a continuous
    ///          (projected) space to the discrete hexagonal grid. It rounds each component
    ///          to the nearest integer, then adjusts the component with the largest rounding
    ///          error to ensure the `i + j + k = 0` invariant is maintained.
    /// @ref H3's `_ijkRound` internal function.
    /// @tparam _Float The floating-point type of the input coordinates (e.g., `double`).
    /// @tparam _Int The integer type for the output coordinates (e.g., `int`).
    /// @param i The floating-point 'i' component.
    /// @param j The floating-point 'j' component.
    /// @param k The floating-point 'k' component.
    /// @param[out] i_out The resulting integer 'i' component.
    /// @param[out] j_out The resulting integer 'j' component.
    /// @param[out] k_out The resulting integer 'k' component.
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
