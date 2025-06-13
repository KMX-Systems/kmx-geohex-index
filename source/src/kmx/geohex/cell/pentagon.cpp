/// @file src/kmx/geohex/cell/pentagon.cpp
/// @ingroup Internal
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
        for (auto base_cell: ids())
        {
            tmp.set_base_cell(base_cell);
            *dest = tmp;
            ++dest;
        }
    }

    static constexpr std::array<clockwise_offsets_t, count> offsets_only_data //
        {{{-1, -1},                                                           // Corresponds to ID 4
          {2, 6},                                                             // Corresponds to ID 14
          {1, 5},                                                             // Corresponds to ID 24
          {3, 7},                                                             // Corresponds to ID 38
          {0, 9},                                                             // Corresponds to ID 49
          {4, 8},                                                             // Corresponds to ID 58
          {11, 15},                                                           // Corresponds to ID 63
          {12, 16},                                                           // Corresponds to ID 72
          {10, 19},                                                           // Corresponds to ID 83
          {13, 17},                                                           // Corresponds to ID 97
          {14, 18},                                                           // Corresponds to ID 117
          {-1, -1}}};                                                         // Corresponds to ID 117

    clockwise_offsets_t clockwise_offsets(const id_t base_cell_id) noexcept
    {
        const auto& pentagon_ids = ids();
        const auto it = std::lower_bound(pentagon_ids.begin(), pentagon_ids.end(), base_cell_id);
        if ((it != pentagon_ids.end()) && (*it == base_cell_id))
        {
            const auto position = std::distance(pentagon_ids.begin(), it);
            return offsets_only_data[position];
        }

        return {}; // Return default/empty if not a pentagon ID.
    }

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

    std::optional<id_t> get_index(const base::id_t global_bc_id) noexcept
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
