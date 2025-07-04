/// @file inc/kmx/geohex/cell/base.hpp
/// @ingroup Internal
/// @brief Provides types, constants, and fundamental utilities for base cells.
#pragma once
#ifndef PCH
    #include <array>
    #include <kmx/geohex/base.hpp>
    #include <kmx/geohex/index.hpp>
#endif

namespace kmx::geohex::cell::base
{
    /// @brief A type alias for a base cell identifier, ranging from 0 to 121.
    using id_t = std::uint8_t;

    /// @brief A type alias for a fixed-size array used to store neighbor data.
    using base_id_array_t = std::array<id_t, 8u>; // one extra item for padding

    /// @brief A sentinel value indicating an invalid base cell index.
    /// @details Used in lookup tables and return values to signify an error or the
    ///          absence of a valid base cell (e.g., a pentagon's missing neighbor).
    constexpr id_t invalid_index = 127u;

    /// @brief Creates a resolution-0 H3 index from a base cell number.
    /// @details This is a convenient factory function for creating the 122 root cells
    ///          of the H3 hierarchy.
    /// @ref baseCellNumToCell
    /// @param no The base cell number (0-121).
    /// @return A valid resolution-0 `kmx::geohex::index` for the given base cell.
    ///         Returns an invalid index if `no` is out of range.
    constexpr index create_index(const id_t no) noexcept;

    /// @brief Finds the neighboring base cell in a given direction.
    /// @details This function performs a single-step traversal on the resolution-0
    ///          base cell graph. It correctly handles pentagons, returning
    ///          `invalid_index` if a traversal is attempted in a pentagon's
    ///          missing neighbor direction.
    /// @param item The ID of the origin base cell (0-121).
    /// @param direction The direction of traversal.
    /// @return The ID of the neighboring base cell, or `invalid_index` on failure
    ///         or if the neighbor does not exist.
    id_t neighbor_of(const id_t item, const direction_t direction);

    /// @brief Finds the direction from an origin base cell to an adjacent neighbor.
    /// @details This is the inverse of `neighbor_of`. It determines which of the
    ///          six neighbor directions connects the two specified base cells.
    /// @param origin The ID of the origin base cell (0-121).
    /// @param neighbor The ID of the destination base cell, which must be an
    ///                 immediate neighbor of the origin.
    /// @return The `direction_t` from origin to neighbor. Returns `direction_t::invalid`
    ///         if the two cells are not immediate neighbors.
    direction_t direction_between(const id_t origin, const id_t neighbor);

    /// @brief Checks if a base cell is a polar pentagon.
    /// @details The two polar pentagons (at the north and south poles) have distinct
    ///          properties in some H3 algorithms.
    /// @ref _isBaseCellPolarPentagon
    /// @param item The ID of the base cell to check.
    /// @return `true` if the base cell is one of the two polar pentagons (4 or 117),
    ///         `false` otherwise.
    constexpr bool is_polar_pentagon(const id_t item) noexcept;

    /// @brief An array holding the number of 60-degree counter-clockwise rotations
    ///        for each of the 7 directions.
    using rotations_60ccw_per_direction_array = std::array<std::int8_t, direction_count>;

    /// @brief Retrieves the rotational adjustments needed when traversing from one base
    ///        cell to another.
    /// @details When moving between two adjacent base cells, the coordinate system may rotate.
    ///          This function provides a lookup table containing the number of 60-degree
    ///          counter-clockwise rotations required to align the coordinate systems for
    ///          a traversal in each of the 6 neighbor directions.
    /// @ref baseCellNeighbor60CCWRots
    /// @param base_cell_id The ID of the origin base cell (0-121).
    /// @return A constant reference to an array where the index corresponds to a
    ///         `direction_t` and the value is the number of CCW rotations.
    const rotations_60ccw_per_direction_array& rotations_60ccw(const id_t base_cell_id) noexcept;

    /// @brief Gets the canonical orientation of a base cell relative to its icosahedron face.
    /// @details In the H3 grid system, some base cells are rotated 60 degrees CCW
    ///          relative to the canonical orientation of their parent icosahedron face.
    ///          This is a fundamental property used in coordinate system transformations.
    /// @ref baseCellData (specifically, the `isRes1Rotated` field in the H3 source).
    /// @param base_cell_id The ID of the base cell.
    /// @return `1` if the base cell is rotated, `0` otherwise.
    constexpr std::int8_t get_canonical_orientation(const id_t base_cell_id) noexcept
    {
        switch (base_cell_id)
        {
            case 8u:
            case 17u:
            case 26u:
            case 35u:
            case 43u:
            case 52u:
            case 62u:
            case 70u:
            case 79u:
            case 88u:
            case 97u:
            case 106u:
            case 115u:
                return 1u;
            default:
                return 0u;
        }
    }
}
