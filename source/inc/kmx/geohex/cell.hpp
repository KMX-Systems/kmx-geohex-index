/// @file inc/kmx/geohex/cell.hpp
/// @ingroup Internal
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
    #include <kmx/math/vector.hpp>
#endif

namespace kmx::geohex::cell
{
    using children_count_t = std::uint64_t;

    constexpr children_count_t base_children_count = 7u;

    /// @brief Adjusts the children count for pentagons.
    constexpr children_count_t basic_children_count(const children_count_t value) noexcept
    {
        return 1u + 5u * (value - 1u) / 6u;
    }

    /// @brief Calculates the number of children a cell has at a finer resolution.
    /// @ref cellToChildrenSize
    /// @param index The parent H3 index.
    /// @param child_resolution The resolution of the children.
    /// @return The number of children.
    children_count_t children_count(const index index, const resolution_t child_resolution) noexcept;
}
