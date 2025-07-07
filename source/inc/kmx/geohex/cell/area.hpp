/// @file inc/kmx/geohex/cell/area.hpp
/// @ingroup Internal
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
#endif

namespace kmx::geohex::cell::area
{
    /// @brief Calculates the area of an cell in square kilometers.
    /// @details This function computes the spherical area of a specific cell using its
    ///          geographic boundary. The calculation correctly accounts for the smaller area of
    ///          pentagonal cells compared to hexagonal cells at the same resolution.
    ///
    ///          This is a non-allocating function that is guaranteed not to throw exceptions.
    ///          Failures, such as an invalid input cell index, are reported via the returned
    ///          `error_t` code.
    ///
    /// @ref cellAreaKm2
    ///
    /// @param cell The cell index for which to calculate the area.
    /// @param[out] out A reference to a float_t that will be filled with the calculated area
    ///                 in square kilometers (km²). The value of this parameter is only
    ///                 valid if the function returns `error_t::none`.
    /// @return `error_t::none` on successful calculation.
    /// @return An error code, such as `error_t::cell_invalid`, if the input index is not
    ///         a valid cell.
    /// @see m2, rad2, index::get_area_km2
    error_t km2(const index cell, float_t& out) noexcept;

    /// @brief Calculates the area of an cell in square meters.
    /// @details This function computes the spherical area of a specific cell using its
    ///          geographic boundary. The calculation correctly accounts for the smaller area of
    ///          pentagonal cells compared to hexagonal cells at the same resolution.
    ///
    ///          This is a non-allocating function that is guaranteed not to throw exceptions.
    ///          Failures, such as an invalid input cell index, are reported via the returned
    ///          `error_t` code.
    ///
    /// @ref cellAreaM2
    ///
    /// @param cell The cell index for which to calculate the area.
    /// @param[out] out A reference to a float_t that will be filled with the calculated area
    ///                 in square meters (m²). The value of this parameter is only
    ///                 valid if the function returns `error_t::none`.
    /// @return `error_t::none` on successful calculation.
    /// @return An error code, such as `error_t::cell_invalid`, if the input index is not
    ///         a valid cell.
    /// @see km2, rad2, index::get_area_m2
    error_t m2(const index cell, float_t& out) noexcept;

    /// @brief Calculates the area of an cell in square radians.
    /// @details This function computes the fundamental spherical area of a specific cell
    ///          in square radians. This unit-agnostic value is the basis for calculating
    ///          areas in other units like square kilometers or meters. The calculation
    ///          correctly accounts for the smaller area of pentagonal cells.
    ///
    ///          This is a core internal primitive that is guaranteed not to throw exceptions.
    ///
    /// @ref cellAreaRads2
    ///
    /// @param cell The cell index for which to calculate the area.
    /// @param[out] out A reference to a float_t that will be filled with the calculated area
    ///                 in square radians (rad²). The value of this parameter is only
    ///                 valid if the function returns `error_t::none`.
    /// @return `error_t::none` on successful calculation.
    /// @return An error code, such as `error_t::cell_invalid`, if the input index is not
    ///         a valid cell.
    /// @see km2, m2
    error_t rad2(const index cell, float_t& out) noexcept;
}
