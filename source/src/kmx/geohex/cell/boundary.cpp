/// @file geohex/cell/boundary.cpp
#include "kmx/geohex/cell/boundary.hpp"
#include "kmx/geohex/icosahedron/face.hpp"
#include <cmath>

namespace kmx::geohex::cell::boundary
{
    error_t get_vertices(const icosahedron::face::ijk& face_ijk, const index index, std::span<gis::wgs84::coordinate> out) noexcept
    {
        // TODO implementation3
        return error_t::none;
    }

    error_t get(const index index, std::span<gis::wgs84::coordinate> out) noexcept
    {
        // Minimum vertices for a valid cell (pentagon has 5).
        // If the output span cannot even hold these, it's an immediate issue.
        if (out.size() < 3u)
            // Smallest H3 polygon needs at least 3 for area, 5 for actual cell
            return error_t::memory_bounds;

        // 1. Validate the H3 cell index (the detail functions might do this too, but good practice here).
        if (!index.is_valid())
            return error_t::cell_invalid;

        // 2. Convert H3 cell index to its center FaceIJK representation.
        icosahedron::face::ijk cell_center_fijk;
        const auto fijk_err = icosahedron::face::from(index, cell_center_fijk);
        if (fijk_err != error_t::none)
            return fijk_err;

        // 3. Get the boundary vertices using the cell's center FaceIJK.
        return get_vertices(cell_center_fijk, index, out);
    }
}
