/// @file geohex/cell/pentagon.hpp
#pragma once
#ifndef PCH
    #include <bitset>
    #include <kmx/geohex/cell.hpp>
    #include <kmx/geohex/cell/base.hpp>
    #include <kmx/geohex/icosahedron/face.hpp>
    #include <kmx/geohex/index.hpp>
    #include <kmx/unsafe_ipow.hpp>
    #include <optional>
#endif

namespace kmx::geohex::cell::pentagon
{
    /// @ref pentagonCount
    constexpr std::uint8_t count = 12u;

    constexpr const std::array<std::uint8_t, count>& indexes() noexcept
    {
        static constexpr std::array<std::uint8_t, count> items //
            {4u, 14u, 24u, 38u, 49u, 58u, 63u, 72u, 83u, 97u, 107u, 117u};
        return items;
    }

    /// @ref _isBaseCellPentagon
    constexpr bool check(const base::id_t item) noexcept
    {
        using bitset_t = std::bitset<128u>;

        static constexpr bitset_t data = []
        {
            bitset_t bits;
            for (auto i: indexes())
                bits.set(i);
            return bits;
        }();

        return item < base::count ? data[item] : false;
    }

    /// @ref getPentagons
    void get(const resolution_t resolution, std::span<index, count> out) noexcept;

    /// @ref cellToChildrenSize
    constexpr std::uint64_t children_count(const resolution_t children_resolution) noexcept
    {
        // The parent is implicitly resolution 0.
        const auto resolution_diff = +children_resolution; // This is (child_res - 0)
        const auto result = unsafe_ipow<std::uint64_t>(base_children_count, resolution_diff);
        return basic_children_count(result);
    }

    /// @ref _faceIjkPentNormalize (H3 C internal)
    /// @brief Normalizes IJK coordinates on a pentagon face, ensuring i != 0, j != 0, k != 0.
    /// @param ijk_coords In/Out: The IJK coordinates to normalize.
    /// @param res The resolution of the cell.
    void normalize(coordinate::ijk& ijk_coords, const resolution_t res) noexcept;

    /// @brief Helper to get the local index (0-11) of a pentagon base cell.
    /// @param global_bc_id The global base cell ID (0-121).
    /// @return The local pentagon index (0-11), or empty optional.
    std::optional<std::uint8_t> get_index(const base::id_t global_bc_id) noexcept;

    using clockwise_offset_t = std::int8_t;
    using counter_clockwise_offset_t = std::int8_t;
    using clockwise_offsets_t = std::pair<clockwise_offset_t, clockwise_offset_t>;

    clockwise_offsets_t clockwise_offsets(const id_t base_cell_id) noexcept;
}
