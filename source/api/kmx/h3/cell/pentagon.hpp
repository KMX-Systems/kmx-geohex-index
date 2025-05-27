/// @file h3/cell/pentagon.hpp
#pragma once
#ifndef PCH
    #include <bitset>
    #include <kmx/h3/cell.hpp>
    #include <kmx/h3/cell/base.hpp>
    #include <kmx/h3/index.hpp>
    #include <kmx/unsafe_ipow.hpp>
#endif

namespace kmx::h3::cell::pentagon
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
}
