/// @file src/kmx/geohex/vertex.cpp
#include "kmx/geohex/vertex.hpp"
#include "kmx/geohex/cell/boundary.hpp"
#include "kmx/geohex/icosahedron/face.hpp"
#include "kmx/geohex/index_helper.hpp"

namespace kmx::geohex::vertex
{
    index from_cell(const index cell, const vertex_no_t vertex_no) noexcept
    {
        // 1. Input Validation
        if (!cell.is_valid() || cell.mode() != index_mode_t::cell)
            return {};

        if (vertex_no > 5u)
            return {};

        // A pentagon has only 5 vertices, numbered 0-4.
        if (cell.is_pentagon() && (vertex_no > 4u))
            return {};

        // 2. Construct the Vertex Index
        // Start with the raw value of the parent cell.
        index_helper vertex_helper {cell.value()};

        // Set the mode to `vertex`.
        vertex_helper.set_mode(index_mode_t::vertex);

        // Encode the logical vertex number into the mode-dependent bits.
        vertex_helper.set_vertex_number(vertex_no);

        // 3. Return the new index
        return index {vertex_helper.value()};
    }

    error_t from_cell(const index cell, std::span<index>& out_vertices) noexcept
    {
        if (!cell.is_valid() || (cell.mode() != index_mode_t::cell))
            return error_t::domain;

        const std::size_t num_vertices = cell.is_pentagon() ? 5u : 6u;
        if (out_vertices.size() < num_vertices)
            return error_t::buffer_too_small;

        // Loop and create each vertex index by calling the single-vertex version.
        for (std::size_t i {}; i < num_vertices; ++i)
            out_vertices[i] = from_cell(cell, static_cast<int>(i));

        // Resize the output span to the exact number of vertices for the cell.
        out_vertices = out_vertices.subspan(0u, num_vertices);
        return error_t::none;
    }

    error_t to_wgs(const index vertex, gis::wgs84::coordinate& out_coord) noexcept
    {
        // 1. Input Validation
        index_helper vertex_helper {vertex.value()};
        if (vertex_helper.mode() != index_mode_t::vertex)
            return error_t::vertex_invalid;

        // 2. Deconstruct the vertex index to find its owner cell and logical number.
        const auto vertex_no = vertex_helper.vertex_number();
        if (vertex_no > 5u)
            return error_t::vertex_invalid;

        vertex_helper.set_mode(index_mode_t::cell);
        const index owner_cell {vertex_helper.value()};
        if (!owner_cell.is_valid())
            return error_t::cell_invalid;

        // 3. Calculate the FaceIJK for this *single vertex* using our new primitive.
        //    This avoids calculating all 6 boundary points.
        icosahedron::face::ijk vertex_fijk;
        const error_t err = cell::boundary::get_vertex_fijk(owner_cell, vertex_no, vertex_fijk);
        if (err != error_t::none)
            return err;

        // 4. Convert the single vertex FaceIJK to a geographic coordinate.
        return icosahedron::face::to_wgs(vertex_fijk, owner_cell.resolution(), out_coord);
    }
}
