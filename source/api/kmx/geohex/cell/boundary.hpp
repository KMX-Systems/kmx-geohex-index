/// @file geohex/cell/base.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/base.hpp>
    #include <kmx/geohex/index.hpp>
#endif

namespace kmx::gis::wgs84
{
    class coordinate;
}

namespace kmx::geohex::icosahedron::face
{
    class ijk;
}

namespace kmx::geohex::cell::boundary
{
    constexpr std::uint8_t max_vertices = 10u;

    error_t get(const index index, std::span<gis::wgs84::coordinate> out) noexcept;

    /// @ref _faceIjkToCellBoundary and _faceIjkPentToCellBoundary (H3 C internal)
    /// @brief Calculates the geographic boundary vertices for a cell given its center FaceIJK.
    /// @param cell_center_fijk The FaceIJK of the cell's center.
    /// @param cell_h3_index The H3 index of the cell (needed for resolution, pentagon status).
    /// @param out_boundary The structure to fill with boundary vertices (in radians, clockwise).
    /// @return error_t::none on success, or an error code.
    error_t get_vertices(const icosahedron::face::ijk& face_ijk, const index index, std::span<gis::wgs84::coordinate> out) noexcept;
}
