/// @file geohex/cell.cpp
#include "kmx/geohex/cell/pentagon.hpp"
#include "kmx/geohex/index.hpp"
#include <kmx/unsafe_ipow.hpp>

namespace kmx::geohex::cell
{
    children_count_t children_count(const index index, const resolution_t child_resolution) noexcept
    {
        const auto resolution_diff = +child_resolution - +index.resolution();
        const auto result = unsafe_ipow<children_count_t>(base_children_count, resolution_diff);
        return pentagon::check(index.base_cell()) ? basic_children_count(result) : result;
    }

    error_t to_wgs84(const index index, gis::wgs84::coordinate& coord) noexcept
    {
        // TODO implement
        return error_t::none;
    }
}
