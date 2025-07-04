/// @file src/kmx/geohex/unidirectional_edge.cpp
#include "kmx/geohex/unidirectional_edge.hpp"
#include "kmx/geohex/grid.hpp"
#include "kmx/geohex/icosahedron/face.hpp"
#include "kmx/geohex/index_helper.hpp"
#include "kmx/geohex/traversal.hpp"
#include "kmx/geohex/vertex.hpp"

namespace kmx::geohex::unidirectional_edge
{
    bool are_neighbors(const index a, const index b) noexcept
    {
        // A robust way to check for neighbors is to see if the grid distance is 1.
        return grid::distance(a, b) == 1;
    }

    [[nodiscard]] index from_cells(const index origin, const index destination) noexcept
    {
        // 1. Input Validation
        if (!origin.is_valid() || !destination.is_valid() || (origin.resolution() != destination.resolution()))
            return {}; // Return invalid index

        // 2. Find the direction from origin to destination using the robust traversal primitive.
        // This correctly handles all icosahedron face crossings and pentagon distortions.
        coordinate::ijk relative_ijk;
        if (index_to_local_ijk(origin, destination, relative_ijk) != error_t::none)
            return {}; // Relationship between cells could not be determined.

        // 3. Convert the resulting local IJK vector into a direction digit.
        // For immediate neighbors, this vector will be a unit vector in one of the 6 directions.
        const direction_t dir = relative_ijk.to_digit();

        // 4. Validate the direction. It must be a valid neighbor direction (K, J, JK, I, IK, IJ).
        if ((dir < direction_t::k_axes) || (dir >= direction_t::invalid))
            return {}; // Not immediate neighbors or an error occurred.

        // 5. Construct the directed edge index.
        index_helper edge_helper {origin.value()};
        edge_helper.set_mode(index_mode_t::edge_unidirectional);
        edge_helper.set_edge_direction(dir); // Use the dedicated helper for safety.

        return index {edge_helper.value()};
    }

    index origin(const index edge) noexcept
    {
        // An edge index is fundamentally an origin cell index with modified mode bits.
        // Return a default-constructed (invalid) index if the input is not a valid edge.
        if (edge.mode() != index_mode_t::edge_unidirectional)
            return {};

        // To get the origin, we simply construct a new helper from the edge's value
        // and change the mode back to `cell`.
        index_helper origin_helper {edge.value()};
        origin_helper.set_mode(index_mode_t::cell);

        return index {origin_helper.value()};
    }

    index destination(const index edge) noexcept
    {
        // 1. Input Validation
        // First, verify that the provided index is actually a valid unidirectional edge.
        // If not, its destination is undefined, so we return a default-constructed index.
        index_helper edge_helper {edge.value()};
        if (edge_helper.mode() != index_mode_t::edge_unidirectional)
            return {};

        // 2. Get Origin Cell and Edge Direction
        // The destination is the neighbor of the origin cell in the direction
        // encoded by the edge.

        // a. Get the origin cell by calling our own `origin` function.
        const index origin_cell = origin(edge);
        if (!origin_cell.is_valid())
            return {}; // This can happen if the edge bits are valid but the underlying cell bits are not.

        // b. Extract the direction from the edge's mode-dependent bits.
        const direction_t dir = edge_helper.edge_direction();
        if (dir == direction_t::invalid || dir == direction_t::center)
            return {}; // An edge cannot have an invalid or center direction.

        // 3. Find the Neighbor
        // The most robust way to find the neighbor is to use the library's core
        // traversal primitive, which handles all face-crossing logic.

        // a. Convert the origin cell to its internal FaceIJK representation.
        icosahedron::face::oriented_ijk origin_fijk;
        if (icosahedron::face::from_index(origin_cell, origin_fijk) != error_t::none)
            return {};

        // b. Find the neighbor's FaceIJK using the core traversal function.
        icosahedron::face::oriented_ijk destination_fijk;
        if (icosahedron::face::find_neighbor_face_ijk(origin_fijk, origin_cell.resolution(), dir, destination_fijk) != error_t::none)
            return {};

        // c. Convert the resulting neighbor FaceIJK back into a final index.
        index destination_cell;
        if (icosahedron::face::to_index(destination_fijk, origin_cell.resolution(), destination_cell) != error_t::none)
            return {};

        // 4. Return Result
        return destination_cell;
    }

    /// @brief A lookup table to map an edge direction to its two bounding vertex numbers.
    /// @details The GeoHex grid has two different orientations (Class II/even and Class III/odd
    ///          resolutions). An edge in a given direction is bounded by a different pair
    ///          of vertices depending on the grid's class. This table stores that mapping.
    ///
    ///          Layout: [is_class_iii][direction][vertex_index (0 or 1)]
    using vertex_pair = std::array<std::int8_t, 2u>;
    using direction_to_vertex_map = std::array<vertex_pair, direction_count + 1u>;
    using edge_vertex_lookup_table = std::array<direction_to_vertex_map, 2u>;

    static constexpr edge_vertex_lookup_table edge_to_vertex_map //
        {{                                                       // Class II (even resolution) Grid Orientation
          {{/* CENTER */ {-1, -1},
            /* K_AXES */ {3, 4},
            /* J_AXES */ {4, 5},
            /* JK_AXES */ {5, 0},
            /* I_AXES */ {0, 1},
            /* IK_AXES */ {1, 2},
            /* IJ_AXES */ {2, 3},
            /* INVALID */ {-1, -1}}},
          // Class III (odd resolution) Grid Orientation
          {{/* CENTER */ {-1, -1},
            /* K_AXES */ {2, 3},
            /* J_AXES */ {3, 4},
            /* JK_AXES */ {4, 5},
            /* I_AXES */ {5, 0},
            /* IK_AXES */ {0, 1},
            /* IJ_AXES */ {1, 2},
            /* INVALID */ {-1, -1}}}}};

    error_t get_boundary(const index edge, std::span<gis::wgs84::coordinate>& out_boundary) noexcept
    {
        // 1. Input Validation
        index_helper edge_helper {edge.value()};
        if (edge_helper.mode() != index_mode_t::edge_unidirectional)
            return error_t::domain;

        // An edge boundary always consists of exactly two vertices.
        if (out_boundary.size() < 2u)
            return error_t::buffer_too_small;

        // 2. Get Edge Properties
        const index origin_cell = origin(edge);
        if (!origin_cell.is_valid())
            return error_t::failed;

        const direction_t dir = edge_helper.edge_direction();
        if (dir >= direction_t::invalid || dir == direction_t::center)
            return error_t::failed; // A valid edge cannot have these directions.

        const std::size_t class_3_index = is_class_3(origin_cell.resolution()) ? 1u : 0u;

        // For pentagons, the 'IJ' direction edge does not exist.
        if (origin_cell.is_pentagon() && dir == direction_t::ij_axes)
            return error_t::pentagon;

        // 3. Determine Bounding Vertex Numbers
        // Use the modern, type-safe std::array lookup table.
        const vertex_pair& vertices = edge_to_vertex_map[class_3_index][static_cast<std::size_t>(dir)];
        const int vertex_num1 = vertices[0u];
        const int vertex_num2 = vertices[1u];

        // 4. Get Vertex Indexes
        // Convert the logical vertex numbers into actual GeoHex vertex indexes.
        const index vertex1 = vertex::from_cell(origin_cell, vertex_num1);
        const index vertex2 = vertex::from_cell(origin_cell, vertex_num2);

        if (!vertex1.is_valid() || !vertex2.is_valid())
            return error_t::failed;

        // 5. Convert Vertex Indexes to Geographic Coordinates
        // Call the vertex API to get the final WGS84 coordinates for each vertex index.
        if (vertex::to_wgs(vertex1, out_boundary[0u]) != error_t::none)
            return error_t::failed;

        if (vertex::to_wgs(vertex2, out_boundary[1u]) != error_t::none)
            return error_t::failed;

        // 6. Finalize Output Span
        // Resize the output span to reflect that we have written exactly two coordinates.
        out_boundary = out_boundary.subspan(0u, 2u);

        return error_t::none;
    }
}
