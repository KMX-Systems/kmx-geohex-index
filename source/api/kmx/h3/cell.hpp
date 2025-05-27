/// @file h3/cell.hpp
#pragma once
#ifndef PCH
    #include <kmx/h3/base.hpp>
    #include <kmx/h3/cell/pentagon.hpp>
    #include <kmx/h3/index.hpp>
#endif

namespace kmx::h3::cell
{
    using children_count_t = std::uint32_t;

    constexpr children_count_t base_children_count = 7u;

    constexpr children_count_t basic_children_count(const children_count_t value) noexcept
    {
        return 1u + 5u * (value - 1u) / 6u;
    }

    /// @ref cellToChildrenSize
    children_count_t children_count(const index index, const resolution_t child_resolution) noexcept;
}
