/// @file geohex/cell/boundary.cpp
#include "kmx/geohex/cell/boundary.hpp"
#include "kmx/geohex/icosahedron/face.hpp"
#include <array>
#include <cmath>

namespace kmx::geohex::cell::boundary
{
    /// @brief Defines the sequence of neighbor directions to traverse to find cell vertices.
    /// @details H3 uses a different vertex ordering for Class II (even) and Class III (odd)
    /// resolutions to maintain grid continuity. This table provides the correct sequence
    /// of single-step `ijk` direction vectors for both cases.
    ///
    /// The vertex directions are ordered to produce a clockwise polygon boundary.
    /// @ref H3 C library constant `VERTEX_DIRECTIONS`.
    static constexpr std::array<std::array<coordinate::ijk, 6u>, 2u> vertex_directions = {
        {// Class II (even resolutions) vertex sequence.
         // Directions: J, IJ, I, IK, K, JK
         {{{0, 1, -1}, {1, 1, -2}, {1, 0, -1}, {2, -1, -1}, {1, -1, 0}, {1, -2, 1}}},
         // Class III (odd resolutions) vertex sequence.
         // Directions: JK, J, IJ, I, IK, K
         {{{1, -2, 1}, {0, 1, -1}, {1, 1, -2}, {1, 0, -1}, {2, -1, -1}, {1, -1, 0}}}}};

    /// @brief Calculates the geographic boundary vertices for a cell given its center FaceIJK.
    /// @details This is the core logic for `cellToBoundary` operations. It determines the
    /// correct vertex sequence based on the cell's resolution class and pentagon status,
    /// then converts the FaceIJK of each vertex into a geographic coordinate.
    ///
    /// The resulting vertices form a clockwise polygon.
    ///
    /// @ref _faceIjkToCellBoundary and _faceIjkPentToCellBoundary (H3 C internal)
    ///
    /// @param center_fijk The FaceIJK of the cell's center.
    /// @param cell_index The H3 index of the cell (needed for resolution and pentagon status).
    /// @param[out] out_vertices A span that will be filled with the boundary vertices.
    ///                        Its size will be adjusted to the number of vertices written.
    /// @return error_t::none on success, or an error code if the output span is too small.
    error_t get_vertices(const icosahedron::face::ijk& center_fijk, const index cell_index,
                         std::span<gis::wgs84::coordinate>& out_vertices) noexcept
    {
        const resolution_t res = cell_index.resolution();
        const bool is_pentagon = cell_index.is_pentagon();
        const auto num_vertices = is_pentagon ? 5u : 6u;

        // Ensure the output span is large enough to hold the result.
        if (out_vertices.size() < num_vertices)
            return error_t::memory_bounds;

        // Select the correct vertex direction sequence based on resolution class.
        // The index `is_class_3(res)` will evaluate to 0 (false) or 1 (true).
        const auto& directions = vertex_directions[is_class_3(res)];

        // This variable tracks how many vertices we have successfully generated.
        std::size_t vertices_written {};

        for (std::size_t i {}; i < num_vertices; ++i)
        {
            // 1. Determine the vertex's FaceIJK coordinates.
            //    Start with the cell center and add the direction vector to the next vertex.
            icosahedron::face::ijk vertex_fijk = center_fijk;
            vertex_fijk.ijk_coords += directions[i];

            // 2. For pentagons, the grid is distorted. When moving from a pentagon
            //    center to a vertex, a counter-clockwise rotation is applied to account
            //    for the "missing" wedge of the grid.
            if (is_pentagon)
                vertex_fijk.ijk_coords.rotate_60ccw();

            // 3. Normalize the vertex coordinates. This is especially important around
            //    pentagons where the addition and rotation may create non-canonical IJK values.
            vertex_fijk.ijk_coords.normalize();

            // 4. Convert the vertex's FaceIJK to a geographic coordinate.
            //    This is the most complex step, involving projection from the icosahedron face
            //    to the sphere. We delegate this to a dedicated function.
            //
            //    NOTE: The `to_wgs` function must correctly handle cases where a vertex
            //    crosses an icosahedron face boundary (overage), which is critical around
            //    pentagons. This implementation assumes `to_wgs` is robust.
            if (icosahedron::face::to_wgs(vertex_fijk, res, out_vertices[vertices_written]) == error_t::none)
                vertices_written++;
            else
            {
                // If any vertex calculation fails, we cannot produce a valid boundary.
                // Clear the output and return an error.
                out_vertices = {};
                return error_t::failed;
            }
        }

        // Adjust the output span's size to match the number of vertices actually written.
        // This is a crucial step for the caller to know the correct boundary size.
        out_vertices = out_vertices.subspan(0, vertices_written);

        return error_t::none;
    }

    // This is the public-facing function from the header.
    // It remains the same, acting as a high-level wrapper.
    error_t get(const index index, std::span<gis::wgs84::coordinate>& out) noexcept
    {
        // 1. Validate the H3 cell index.
        if (!index.is_valid())
            return error_t::cell_invalid;

        // 2. Convert H3 cell index to its center FaceIJK representation.
        icosahedron::face::ijk cell_center_fijk;
        const auto fijk_err = icosahedron::face::from_index(index, cell_center_fijk);
        if (fijk_err != error_t::none)
            return fijk_err;

        // 3. Get the boundary vertices using the core logic.
        return get_vertices(cell_center_fijk, index, out);
    }
}
