/// @file inc/kmx/geohex/cell/base.hpp
/// @ingroup Internal
#pragma once
#ifndef PCH
    #include <array>
    #include <kmx/geohex/base.hpp>
    #include <kmx/geohex/index.hpp>
#endif

namespace kmx::geohex::cell::base
{
    using id_t = std::uint8_t;
    using base_id_array_t = std::array<id_t, 8u>; // one extra item for padding

    constexpr id_t invalid_index = 127u;

    /// @ref baseCellNumToCell
    constexpr index create_index(const id_t no) noexcept
    {
        index result;
        if (no < count)
            result.set_base_cell(no);
        return result;
    }

    /// @ref
    id_t neighbor_of(const id_t item, const direction_t direction);

    /// @ref
    direction_t direction_between(const id_t origin, const id_t neightbor);

    /// @ref _isBaseCellPolarPentagon
    constexpr bool is_polar_pentagon(const id_t item) noexcept
    {
        return (item == 4u) || (item == 117u);
    }

    using rotations_60ccw_per_direction_array = std::array<std::int8_t, direction_count>;

    /// @ref baseCellNeighbor60CCWRots
    const rotations_60ccw_per_direction_array& rotations_60ccw(const id_t base_cell_id) noexcept;

    /// @brief Gets the canonical counter-clockwise rotation of a base cell relative to its face.
    /// @ref baseCellData
    /// @param base_cell_id The ID of the base cell.
    /// @return 1 if the base cell is rotated, 0 otherwise.
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
