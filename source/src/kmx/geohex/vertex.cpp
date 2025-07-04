/// @file src/kmx/geohex/vertex.cpp
#include "kmx/geohex/vertex.hpp"
#include "kmx/geohex/cell/boundary.hpp"
#include "kmx/geohex/index_helper.hpp"

namespace kmx::geohex::vertex
{
    index from_cell(const index cell, const std::uint8_t vertex_num) noexcept
    {
        // 1. Input Validation
        if (!cell.is_valid() || cell.mode() != index_mode_t::cell)
            return {};

        if (vertex_num > 5u)
            return {};

        // A pentagon has only 5 vertices, numbered 0-4.
        if (cell.is_pentagon() && (vertex_num > 4u))
            return {};

        // 2. Construct the Vertex Index
        // Start with the raw value of the parent cell.
        index_helper vertex_helper {cell.value()};

        // Set the mode to `vertex`.
        vertex_helper.set_mode(index_mode_t::vertex);

        // Encode the logical vertex number into the mode-dependent bits.
        vertex_helper.set_vertex_number(vertex_num);

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
            return error_t::domain;

        // 2. Determine the "Owner" Cell and Vertex Number
        // A vertex index is derived from a single "owner" cell. We can reverse
        // the creation process to find it.

        // a. Extract the logical vertex number from the mode-dependent bits.
        const int vertex_num = vertex_helper.vertex_number();
        if (vertex_num > 5) // Vertex numbers are only 0-5.
            return error_t::cell_invalid;

        // b. Get the owner cell by changing the mode back to `cell`.
        vertex_helper.set_mode(index_mode_t::cell);
        const index owner_cell {vertex_helper.value()};
        if (!owner_cell.is_valid())
            return error_t::cell_invalid;

        // 3. Get the Geographic Boundary of the Owner Cell
        // The geographic location of the vertex is simply one of the points
        // on the owner cell's boundary.

        // a. Create a buffer on the stack to hold the boundary coordinates.
        //    The max size is always sufficient.
        std::array<gis::wgs84::coordinate, cell::boundary::max_vertices> boundary_buffer;
        std::span<gis::wgs84::coordinate> boundary_span = boundary_buffer;

        // b. Call the cell boundary primitive.
        const error_t boundary_err = cell::boundary::get(owner_cell, boundary_span);
        if (boundary_err != error_t::none)
            return boundary_err;

        // 4. Select the Correct Vertex from the Boundary
        // `boundary_span` is now resized to the actual number of vertices (5 or 6).
        // Check if the requested vertex number is valid for this specific cell.
        if (static_cast<std::size_t>(vertex_num) >= boundary_span.size())
            return error_t::cell_invalid; // This can happen if a vertex index was manually
                                          // constructed from a pentagon with vertex_num=5.

        // The vertex number directly corresponds to the index in the canonical boundary array.
        out_coord = boundary_span[vertex_num];

        return error_t::none;
    }
}
