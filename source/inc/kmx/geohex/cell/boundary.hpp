/// @file inc/kmx/geohex/cell/boundary.hpp
/// @ingroup Internal
/// @brief Provides internal functions for calculating the geographic boundaries of cells.
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
#endif

// Forward declarations to avoid including full headers in this internal header.
namespace kmx::gis::wgs84
{
    class coordinate;
}

namespace kmx::geohex::icosahedron::face
{
    struct ijk;
}

namespace kmx::geohex::cell::boundary
{
    /// @brief Gets the geographic boundary vertices of an cell.
    /// @details This is the primary internal entry point for boundary calculation. It
    ///          orchestrates the conversion of an index to its internal `FaceIJK`
    ///          representation and then calls the core `get_vertices` worker function.
    /// @param index The cell index.
    /// @param[out] out A span to be filled with the boundary vertices, ordered clockwise.
    ///                 The span must have a capacity of at least `cell::boundary::max_vertices` (10)
    ///                 to guarantee safety. On success, it will be resized to the actual
    ///                 number of vertices found (6 for a hexagon, 5 for a pentagon).
    /// @return `error_t::none` on success.
    /// @return `error_t::cell_invalid` if the input index is not a valid cell.
    /// @return `error_t::buffer_too_small` if the output span is insufficient.
    error_t get(const index index, std::span<gis::wgs84::coordinate>& out) noexcept;

    /// @brief Calculates the geographic boundary vertices for a cell given its center FaceIJK.
    /// @details This is the core "worker" function for `cellToBoundary` operations. It determines
    ///          the correct vertex sequence based on the cell's resolution class and pentagon
    ///          status, then converts the `FaceIJK` of each vertex into a geographic coordinate.
    ///          The resulting vertices form a clockwise polygon.
    /// @ref _faceIjkToCellBoundary and _faceIjkPentToCellBoundary (H3 C internal)
    /// @param center_fijk The FaceIJK of the cell's center.
    /// @param cell_index The index of the cell (needed for resolution and pentagon status).
    /// @param[out] out_vertices A span that will be filled with the boundary vertices.
    ///                        Its size will be adjusted to the number of vertices written.
    /// @return `error_t::none` on success, or an error code if the output span is too small.
    error_t get_vertices(const icosahedron::face::ijk& center_fijk, const index cell_index,
                         std::span<gis::wgs84::coordinate>& out_vertices) noexcept;

    /// @brief Calculates the FaceIJK of a single vertex of a cell.
    /// @details This is a core optimization primitive that avoids calculating the entire
    ///          cell boundary when only one vertex's coordinates are needed. It is significantly
    ///          more efficient than calling `get_vertices` and selecting a single point.
    ///          It is used by `vertex::to_wgs` to achieve high performance.
    /// @param cell_index The index of the cell.
    /// @param vertex_num The vertex number to get (0-5, or 0-4 for pentagons).
    /// @param[out] out_vertex_fijk The structure to fill with the vertex's `FaceIJK` coordinates.
    ///                             The value is only valid if the function returns `error_t::none`.
    /// @return `error_t::none` on success.
    /// @return `error_t::cell_invalid` if `cell_index` is invalid.
    /// @return `error_t::vertex_invalid` if `vertex_num` is out of range for the given cell.
    error_t get_vertex_fijk(const index cell_index, std::uint8_t vertex_num, icosahedron::face::ijk& out_vertex_fijk) noexcept;
}
