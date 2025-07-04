/// @file inc/kmx/geohex/cell/pentagon.hpp
/// @ingroup Internal
/// @brief Provides types, constants, and utilities for handling H3 pentagonal cells.
/// @details The H3 grid is based on a spherical icosahedron, which requires 12 pentagons
///          (one at each icosahedron vertex) to tile the sphere. These pentagons have
///          distinct properties from hexagonal cells (e.g., only 5 neighbors, different
///          area, distorted child cells). This file provides the core internal tools
///          to manage these special cases.
#pragma once
#ifndef PCH
    #include <bitset>
    #include <kmx/geohex/cell.hpp>
    #include <kmx/geohex/cell/base.hpp>
    #include <kmx/geohex/coordinate/ijk.hpp>
    #include <kmx/geohex/icosahedron/face.hpp>
    #include <kmx/geohex/index.hpp>
    #include <kmx/unsafe_ipow.hpp>
    #include <optional>
#endif

namespace kmx::geohex::cell::pentagon
{
    /// @brief A type alias for a pentagonal base cell identifier.
    using id_t = base::id_t;

    /// @brief The total number of pentagons in the H3 grid system.
    /// @ref pentagonCount
    constexpr std::uint8_t count = 12u;

    /// @brief Provides a constant reference to the array of the 12 pentagonal base cell IDs.
    /// @return A constant reference to a `std::array` containing the IDs of the 12 base cells
    ///         that are pentagons.
    constexpr const std::array<id_t, count>& ids() noexcept
    {
        static constexpr std::array<id_t, count> items //
            {4u, 14u, 24u, 38u, 49u, 58u, 63u, 72u, 83u, 97u, 107u, 117u};
        return items;
    }

    /// @brief Checks if a given base cell ID corresponds to a pentagon.
    /// @details This is a high-performance check using a pre-computed `std::bitset` for
    ///          O(1) lookup time.
    /// @ref _isBaseCellPentagon
    /// @param item The base cell ID (0-121) to check.
    /// @return `true` if the base cell is a pentagon, `false` otherwise.
    constexpr bool check(const base::id_t item) noexcept
    {
        using bitset_t = std::bitset<128u>;

        static constexpr bitset_t data = []
        {
            bitset_t bits;
            for (auto i: ids())
                bits.set(i);
            return bits;
        }();

        return item < base::count ? data[item] : false;
    }

    /// @brief Fills a span with the H3 indexes of all 12 pentagons at a given resolution.
    /// @ref getPentagons
    /// @param resolution The target H3 resolution.
    /// @param[out] out A span of exactly 12 `index` objects that will be filled with the
    ///               pentagon indexes for the specified resolution.
    void get(const resolution_t resolution, std::span<index, count> out) noexcept;

    /// @brief Calculates the number of children a pentagon has at a finer resolution.
    /// @details A pentagon has fewer children than a hexagon. This function applies the
    ///          correct formula to determine the exact number of child cells. The parent
    ///          is implicitly assumed to be a pentagon at resolution 0.
    /// @ref cellToChildrenSize (when the parent is a pentagon).
    /// @param children_resolution The target resolution for the children (must be > 0).
    /// @return The total number of children a pentagon has at the specified resolution.
    constexpr std::uint64_t children_count(const resolution_t children_resolution) noexcept
    {
        // The parent is implicitly resolution 0.
        const auto resolution_diff = +children_resolution; // This is (child_res - 0)
        const auto result = unsafe_ipow<std::uint64_t>(base_children_count, resolution_diff);
        return basic_children_count(result);
    }

    /// @brief Normalizes IJK coordinates for use on a pentagon's distorted grid.
    /// @details Due to the geometric distortion around a pentagon, child cell coordinates
    ///          can fall on an axis (e.g., i=0), which is an invalid state. This function
    ///          corrects this by "pulling" the coordinate off the axis toward the
    ///          pentagon's center, and then re-normalizes it to ensure the `i+j+k=0`
    ///          invariant is maintained. This is a critical step in any algorithm that
    ///          descends the grid hierarchy into a pentagon.
    /// @ref _faceIjkPentNormalize
    /// @param[in,out] ijk_coords The IJK coordinates to normalize.
    void normalize(coordinate::ijk& ijk_coords) noexcept;

    /// @brief Gets the local index (0-11) of a pentagon base cell from its global ID.
    /// @param global_bc_id The global base cell ID (0-121).
    /// @return An `std::optional` containing the local pentagon index (0-11) if the
    ///         input ID corresponds to a pentagon. Returns `std::nullopt` otherwise.
    std::optional<id_t> get_index(const base::id_t global_bc_id) noexcept;

    /// @brief Type alias for a clockwise rotational offset.
    using clockwise_offset_t = std::int8_t;

    /// @brief Type alias for a counter-clockwise rotational offset.
    using counter_clockwise_offset_t = std::int8_t;

    /// @brief A pair of clockwise rotational offsets associated with a pentagon's faces.
    /// @details This is part of the data required to handle coordinate system rotations when
    ///          traversing across icosahedron face boundaries near a pentagon.
    /// @see H3 internal `pentagonCounterClockwiseOffsets`.
    using clockwise_offsets_t = std::pair<clockwise_offset_t, clockwise_offset_t>;

    /// @brief Retrieves the pre-calculated clockwise rotational offsets for a pentagon.
    /// @details When traversing from a pentagon base cell to an adjacent icosahedron face,
    ///          a specific rotational adjustment must be applied to the coordinate system.
    ///          This function provides these baked-in offsets.
    /// @ref H3 internal `_pentagonCwOffset` logic.
    /// @param base_cell_id The global ID of the pentagon base cell.
    /// @return A `clockwise_offsets_t` pair containing the rotational adjustments.
    ///         Returns a default-constructed (zeroed) pair if the input is not a pentagon.
    clockwise_offsets_t clockwise_offsets(const id_t base_cell_id) noexcept;
}
