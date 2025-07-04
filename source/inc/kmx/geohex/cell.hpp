/// @file inc/kmx/geohex/cell.hpp
/// @ingroup Internal
/// @brief Provides general types, constants, and utilities for cells.
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
    #include <kmx/math/vector.hpp>
#endif

namespace kmx::geohex::cell
{
    /// @brief A type alias for the count of child cells.
    /// @details Uses `std::uint64_t` to accommodate the very large number of children
    ///          that a coarse cell can have at the finest resolutions.
    using children_count_t = std::uint64_t;

    /// @brief The number of children a hexagonal cell has at the next immediate finer resolution.
    /// @details This constant represents the "aperture" of the H3 grid system, which is 7.
    ///          It is the base for all child count calculations.
    constexpr children_count_t base_children_count = 7u;

    /// @brief Adjusts a theoretical children count for a pentagonal cell.
    /// @details This function applies a correction formula to an children count that was
    ///          calculated using powers of 7. Since pentagons have fewer children than
    ///          hexagons, this formula scales the number down appropriately.
    /// @param value The theoretical number of children, calculated as if the parent were a hexagon.
    /// @return The corrected number of children for a pentagonal parent.
    constexpr children_count_t basic_children_count(const children_count_t value) noexcept
    {
        return 1u + 5u * (value - 1u) / 6u;
    }

    /// @brief Calculates the exact number of children a cell has at a finer resolution.
    /// @details This function determines the total number of cells at a specified finer
    ///          resolution that are contained within a single parent cell. It correctly
    ///          dispatches to the appropriate logic for hexagonal and pentagonal parent
    ///          cells.
    /// @ref cellToChildrenSize
    /// @param index The parent H3 cell index.
    /// @param child_resolution The resolution of the children (must be finer than the
    ///                         parent's resolution).
    /// @return The exact number of children. Returns 0 if `child_resolution` is not
    ///         finer than the parent's resolution.
    children_count_t children_count(const index index, const resolution_t child_resolution) noexcept;
}
