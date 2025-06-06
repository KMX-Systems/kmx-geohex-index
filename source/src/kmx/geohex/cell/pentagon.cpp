/// @file geohex/cell/pentagon.cpp
#include "kmx/geohex/cell/pentagon.hpp"
#include "kmx/geohex/icosahedron/face.hpp"
#include <optional>

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
        for (auto base_cell: indexes())
        {
            tmp.set_base_cell(base_cell);
            *dest = tmp;
            ++dest;
        }
    }

    /// @ref _faceIjkPentNormalize
    void normalize(coordinate::ijk& ijk_coords, const resolution_t res) noexcept
    {
        // Port of _faceIjkPentNormalize from H3 C.
        // This ensures that after all transformations, if an IJK coordinate on a pentagon
        // ends up with a 0 in one of its components (which is normally invalid for pentagon centers),
        // it's adjusted. Typically, this involves "pulling" it towards the center of the pentagon's
        // local coordinate system.

        // TODO: Implement the logic from H3 C.
        // It typically involves checks like:
        // if (ijk_coords.i == 0) { ijk_coords.i = ...; ijk_coords.j -= ...; ijk_coords.k -= ...; }
        // The exact adjustment depends on which component is zero and the resolution class.
        // It uses `ijk.normalize()` after adjustments.
        return; // Stub
    }
}
