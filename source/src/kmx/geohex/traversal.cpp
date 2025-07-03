/// @file src/kmx/geohex/traversal.cpp
#include "kmx/geohex/traversal.hpp"
#include "kmx/geohex/icosahedron/face.hpp"
#include <queue>

namespace kmx::geohex
{
    error_t get_neighbors(const index origin, std::span<index>& out_neighbors) noexcept
    {
        // 1. A buffer of size 6 is always required for the output.
        if (out_neighbors.size() < 6u)
            return error_t::buffer_too_small;

        // 2. The core algorithm requires the origin's FaceIJK representation.
        icosahedron::face::oriented_ijk origin_fijk;
        if (icosahedron::face::from_index(origin, origin_fijk) != error_t::none)
            return error_t::failed;

        std::size_t neighbors_written {};
        const resolution_t res = origin.resolution();

        // 3. Iterate through the 6 valid neighbor directions (K, J, JK, I, IK, IJ).
        for (std::uint8_t i = 1u; i <= 6u; ++i)
        {
            const direction_t dir = static_cast<direction_t>(i);

            // a. Find the FaceIJK of the neighbor in the given direction.
            // This is the most critical step, as it handles all face-crossing logic.
            icosahedron::face::oriented_ijk neighbor_fijk;
            const error_t neighbor_err = icosahedron::face::find_neighbor_face_ijk(origin_fijk, res, dir, neighbor_fijk);

            // For pentagons, one direction will lead to no neighbor. `find_neighbor_face_ijk`
            // should return a specific error for this case (e.g., error_t::pentagon).
            // We treat it as a valid outcome and simply skip that direction.
            if (neighbor_err == error_t::pentagon)
                continue;

            if (neighbor_err != error_t::none)
                return neighbor_err; // A real failure occurred.

            // b. Convert the resulting FaceIJK back into a final index.
            index neighbor_index;
            if (icosahedron::face::to_index(neighbor_fijk, res, neighbor_index) != error_t::none)
                return error_t::failed;

            // c. Store the valid neighbor index in the output buffer.
            out_neighbors[neighbors_written++] = neighbor_index;
        }

        // 4. Resize the output span to the actual number of neighbors found.
        // This will be 6 for a hexagon and 5 for a pentagon.
        out_neighbors = out_neighbors.subspan(0u, neighbors_written);
        return error_t::none;
    }

    error_t index_to_local_ijk(const index& origin, const index& target, coordinate::ijk& out_ijk) noexcept
    {
        const resolution_t res = origin.resolution();

        // 1. Validate inputs.
        if (res != target.resolution() || !origin.is_valid() || !target.is_valid())
            return error_t::res_mismatch;

        const auto origin_bc = origin.base_cell();
        const auto target_bc = target.base_cell();

        // 2. Simple case: Same base cell domain.
        if (origin_bc == target_bc)
        {
            icosahedron::face::ijk origin_fijk, target_fijk;
            if (icosahedron::face::from_index(origin, origin_fijk) != error_t::none ||
                icosahedron::face::from_index(target, target_fijk) != error_t::none)
                return error_t::failed;

            out_ijk = target_fijk.ijk_coords - origin_fijk.ijk_coords;
            return error_t::none;
        }

        // 3. Complex case: Different base cell domains.

        // a. Find the shortest path on the base cell graph (same as before).
        std::queue<cell::base::id_t> q;
        q.push(origin_bc);
        std::array<cell::base::id_t, cell::base::count> parent_map;
        parent_map.fill(cell::base::invalid_index);
        parent_map[origin_bc] = origin_bc;

        bool path_found = false;
        while (!q.empty())
        {
            const cell::base::id_t current_bc = q.front();
            q.pop();

            if (current_bc == target_bc)
            {
                path_found = true;
                break;
            }

            for (uint8_t i = 1; i < direction_count; ++i)
            {
                const direction_t dir = static_cast<direction_t>(i);
                const cell::base::id_t neighbor_bc = cell::base::neighbor_of(current_bc, dir);
                if (neighbor_bc != cell::base::invalid_index && parent_map[neighbor_bc] == cell::base::invalid_index)
                {
                    parent_map[neighbor_bc] = current_bc;
                    q.push(neighbor_bc);
                }
            }
        }
        if (!path_found)
            return error_t::failed;

        // b. Convert the target index to its absolute FaceIJK representation.
        icosahedron::face::oriented_ijk transformed_fijk;
        if (icosahedron::face::from_index(target, transformed_fijk) != error_t::none)
            return error_t::failed;

        // c. Traverse the path backwards from target to origin, transforming the
        //    FaceIJK at each step using the library's own neighbor-finding logic.
        cell::base::id_t current_path_bc = target_bc;
        while (current_path_bc != origin_bc)
        {
            const cell::base::id_t parent_bc = parent_map[current_path_bc];
            const direction_t forward_dir = cell::base::direction_between(parent_bc, current_path_bc);

            // The direction for the *reverse* step is needed to move from the current
            // cell back to its parent in the path. This requires rotating the forward
            // direction by 180 degrees (3 * 60deg clockwise).
            direction_t reverse_dir = rotate_60cw(rotate_60cw(rotate_60cw(forward_dir)));

            // This is the key step: use the grid traversal primitive to move the
            // FaceIJK from one base cell domain to the next. This function correctly
            // handles all rotations and face-crossing adjustments.
            icosahedron::face::oriented_ijk temp_fijk;
            if (icosahedron::face::find_neighbor_face_ijk(transformed_fijk, res, reverse_dir, temp_fijk) != error_t::none)
                return error_t::failed;

            transformed_fijk = temp_fijk;
            current_path_bc = parent_bc;
        }

        // d. At this point, `transformed_fijk` is in the same coordinate system as the origin.
        //    Now we can do the final subtraction.
        icosahedron::face::ijk origin_fijk;
        if (icosahedron::face::from_index(origin, origin_fijk) != error_t::none)
            return error_t::failed;

        out_ijk = transformed_fijk.ijk_coords - origin_fijk.ijk_coords;
        return error_t::none;
    }

    error_t local_ijk_to_index(const index origin, const coordinate::ijk& ijk, index& out_index) noexcept
    {
        // 1. Get the absolute FaceIJK of the origin
        icosahedron::face::oriented_ijk origin_fijk;
        if (icosahedron::face::from_index(origin, origin_fijk) != error_t::none)
            return error_t::failed;

        // 2. Add the local IJK offset to the origin's IJK to find the target
        // This gives us a FaceIJK that is *conceptually* correct but may be outside
        // the coordinate bounds of the origin's immediate face.
        icosahedron::face::oriented_ijk target_fijk = origin_fijk;
        target_fijk.ijk_coords += ijk;
        target_fijk.ijk_coords.normalize();

        // 3. Traverse from origin to target to resolve face crossings
        // We now have the start and end points in the same conceptual grid. We perform
        // a line-drawing traversal *again*, but this time our goal is not to find the
        // path, but to correctly accumulate the coordinate transformations that occur
        // when crossing icosahedron faces.

        const resolution_t res = origin.resolution();
        const int dist = ijk.distance_to({0, 0, 0});
        if (dist == 0)
        {
            out_index = origin;
            return error_t::none;
        }

        // We start at the origin's FaceIJK and step towards the target.
        icosahedron::face::oriented_ijk current_fijk = origin_fijk;

        // Pre-calculate interpolation steps for the traversal direction.
        const double i_step = static_cast<double>(ijk.i) / dist;
        const double j_step = static_cast<double>(ijk.j) / dist;
        const double k_step = static_cast<double>(ijk.k) / dist;

        for (int i = 1; i <= dist; ++i)
        {
            // Find the ideal intermediate point for this step.
            const double i_frac = i_step * i;
            const double j_frac = j_step * i;
            const double k_frac = k_step * i;

            // Round it to find the next cell center on the line.
            coordinate::ijk::value i_round, j_round, k_round;
            cube_round(i_frac, j_frac, k_frac, i_round, j_round, k_round);
            const coordinate::ijk next_local_ijk {i_round, j_round, k_round};

            // Convert the local IJK coordinates of the previous and next cells
            // into IJK vectors relative to the origin's canonical grid.
            const coordinate::ijk last_global_ijk = current_fijk.ijk_coords;
            const coordinate::ijk next_global_ijk = origin_fijk.ijk_coords + next_local_ijk;

            // The direction of this step is the difference between the two global IJKs.
            const coordinate::ijk diff = next_global_ijk - last_global_ijk;
            const direction_t step_dir = diff.to_digit();

            if (step_dir == direction_t::center)
                continue; // No movement, duplicate point.
            if (step_dir == direction_t::invalid)
                return error_t::failed; // Should not happen.

            // Use the core traversal primitive to take one step. This function
            // correctly updates `current_fijk` across any face boundaries.
            icosahedron::face::oriented_ijk temp_fijk;
            if (icosahedron::face::find_neighbor_face_ijk(current_fijk, res, step_dir, temp_fijk) != error_t::none)
                return error_t::failed;

            current_fijk = temp_fijk;
        }

        // 4. Convert the final, globally-correct FaceIJK to an index
        return icosahedron::face::to_index(current_fijk, res, out_index);
    }
}
