/// @file h3/cell/base.hpp
#pragma once
#ifndef PCH
    #include <array>
    #include <kmx/h3/base.hpp>
    #include <kmx/h3/index.hpp>
#endif

namespace kmx::h3::cell::base
{
    using id_t = std::uint8_t;
    using base_id_array_t = std::array<id_t, 8u>; // one extra item for padding

    /// @ref res0CellCount
    constexpr id_t count = 122u;

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

    using rotations_60ccw_per_direction_array = std::array<std::int8_t, 7u>;

    /// @ref
    const rotations_60ccw_per_direction_array& rotations_60ccw(const id_t base_cell_id);

    using clockwise_offset_t = std::int8_t;
    using clockwise_offsets_t = std::pair<clockwise_offset_t, clockwise_offset_t>;

    clockwise_offsets_t clockwise_offsets(const id_t base_cell_id) noexcept;
}
