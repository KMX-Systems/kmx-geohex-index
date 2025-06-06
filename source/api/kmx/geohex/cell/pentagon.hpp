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
        static constexpr std::array<std::uint8_t, count> items {4u, 14u, 24u, 38u, 49u, 58u, 63u, 72u, 83u, 97u, 107u, 117u};
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

        return item < count ? data[item] : false;
    }

    /// @ref getPentagons
    void get(const resolution_t resolution, std::span<index, count> out) noexcept;

    /// @ref cellToChildrenSize
    constexpr children_count_t children_count(const resolution_t children_resolution) noexcept
    {
        const auto resolution_diff = +children_resolution - resolution_count;
        return basic_children_count(unsafe_ipow<children_count_t>(base_children_count, resolution_diff));
    }

    /// @ref _faceIjkPentNormalize (H3 C internal)
    /// @brief Normalizes IJK coordinates on a pentagon face, ensuring i != 0, j != 0, k != 0.
    /// @param ijk_coords In/Out: The IJK coordinates to normalize.
    /// @param res The resolution of the cell.
    void normalize(coordinate::ijk& ijk_coords, const resolution_t res) noexcept;

    /// @brief Helper to get the local index (0-11) of a pentagon base cell.
    /// @param global_bc_id The global base cell ID (0-121).
    /// @return The local pentagon index (0-11), or -1 if not a pentagon.
    int get_local_pentagon_index(cell::base::id_t global_bc_id) noexcept;
}
