/// @file geohex/cell/pentagon.cpp
#include "kmx/geohex/cell/pentagon.hpp"
#include <algorithm>

namespace kmx::geohex::cell::pentagon
{
    constexpr std::uint8_t get_max_coord_for_face(const resolution_t resolution) noexcept
    {
        const auto value = +resolution;
        return (value != 0u) ? value : 1u;
    }

    void get(const resolution_t resolution, std::span<index, count> out) noexcept
    {
        index tmp;
        tmp.set_mode(index_mode_t::cell);
        tmp.set_resolution(resolution);

        auto dest = out.begin();
        for (auto base_cell: indexes())
        {
            tmp.set_base_cell(base_cell);
            *dest = tmp;
            ++dest;
        }
    }

    /// @brief The data is stored in a `constexpr` array, sorted by the pentagon's base cell ID.
    static constexpr std::array<std::pair<id_t, clockwise_offsets_t>, count> clockwise_offsets_data //
        {{{4u, {-1, -1}},
          {14u, {2, 6}},
          {24u, {1, 5}},
          {38u, {3, 7}},
          {49u, {0, 9}},
          {58u, {4, 8}},
          {63u, {11, 15}},
          {72u, {12, 16}},
          {83u, {10, 19}},
          {97u, {13, 17}},
          {107u, {14, 18}},
          {117u, {-1, -1}}}};

    /// @brief A comparator for use with std::lower_bound.
    struct pair_comparator
    {
        constexpr bool operator()(const std::pair<id_t, clockwise_offsets_t>& pair, id_t value) const noexcept
        {
            return pair.first < value;
        }
    };

    clockwise_offsets_t clockwise_offsets(const id_t base_cell_id) noexcept
    {
        // Use std::lower_bound to perform a binary search on our constexpr sorted array.
        const auto it = std::lower_bound(clockwise_offsets_data.begin(), clockwise_offsets_data.end(), base_cell_id, pair_comparator {});

        // Check if we found an exact match.
        if ((it != clockwise_offsets_data.end()) && (it->first == base_cell_id))
            return it->second;

        return {}; // Not found
    }

    /// @brief Normalizes IJK coordinates for use on a pentagon face.
    /// @details This is a two-step process for pentagons:
    ///          1. **Axis Correction:** It first corrects for the geometrically invalid state
    ///             where a coordinate lies on an axis (a component is zero). It does this by
    ///             "pulling" the coordinate off the axis towards the pentagon's center.
    ///          2. **Canonicalization:** The axis correction may invalidate the `i+j+k=0`
    ///             invariant. Therefore, this function concludes by calling the standard
    ///             `coordinate::ijk::normalize()` to restore the algebraic invariant,
    ///             snapping the coordinate to the nearest valid grid center.
    ///
    /// This function should be called for any coordinate that is a child of a pentagon.
    ///
    /// @ref _faceIjkPentNormalize
    /// @param ijk_coords The IJK coordinates to normalize (in/out).
    void normalize(coordinate::ijk& ijk_coords) noexcept
    {
        // Step 1: Fix the pentagon-specific "on-axis" problem.
        if ((ijk_coords.i == 0) || (ijk_coords.j == 0) || (ijk_coords.k == 0))
        {
            // "Pull" the coordinate towards the center by adding the smallest component
            // (which will be negative) to all three components.
            const auto min_val = std::min({ijk_coords.i, ijk_coords.j, ijk_coords.k});
            ijk_coords.i += min_val;
            ijk_coords.j += min_val;
            ijk_coords.k += min_val;
        }

        // Step 2: Restore the i + j + k = 0 invariant.
        ijk_coords.normalize();
    }

    std::optional<std::uint8_t> get_index(const base::id_t global_bc_id) noexcept
    {
        switch (global_bc_id)
        {
            case 4u:
                return 0u;
            case 14u:
                return 1u;
            case 24u:
                return 2u;
            case 38u:
                return 3u;
            case 49u:
                return 4u;
            case 58u:
                return 5u;
            case 63u:
                return 6u;
            case 72u:
                return 7u;
            case 83u:
                return 8u;
            case 97u:
                return 9u;
            case 107u:
                return 10u;
            case 117u:
                return 11u;
            default:
                return {};
        }
    }
}
