/// @file src/kmx/geohex/grid.cpp
#include "kmx/geohex/grid.hpp"
#include "kmx/geohex/icosahedron/face.hpp" // For H3's core algorithms
#include "kmx/geohex/index_utils.hpp"
#include <unordered_set>

namespace kmx::geohex::grid
{
    int distance(const index a, const index b) noexcept
    {
        coordinate::ijk relative_ijk;
        // The public function's logic is correct: delegate and calculate.
        if (index_to_local_ijk(a, b, relative_ijk) != error_t::none)
            return -1; // Error indicates distance cannot be computed.

        // The hexagonal grid distance is the largest of the absolute differences
        // of the IJK components, which is what `distance_to({0,0,0})` computes.
        return relative_ijk.distance_to({0, 0, 0});
    }

    error_t k_ring(const index origin, int k, std::span<index>& out_ring) noexcept
    {
        if (k < 0 || !origin.is_valid())
            return error_t::domain;

        const std::size_t max_size = max_k_ring_size(k);
        if (out_ring.size() < max_size)
            return error_t::buffer_too_small;

        // The k-ring algorithm is a breadth-first search on the grid.
        std::size_t count {};
        out_ring[count++] = origin;

        // Use a hash set for efficient O(1) average time complexity checks for visited cells.
        std::unordered_set<index> visited;
        visited.insert(origin);

        // A temporary vector to hold the cells of the current ring being expanded.
        std::vector<index> current_ring_frontier;
        current_ring_frontier.push_back(origin);

        // A buffer to hold neighbors for each cell. Size 6 is always sufficient.
        std::array<index, 6u> neighbor_buffer;

        for (int ring_k = 1; ring_k <= k; ++ring_k)
        {
            std::vector<index> next_ring_frontier;
            next_ring_frontier.reserve(current_ring_frontier.size() * 6u);

            for (const index cell: current_ring_frontier)
            {
                // Prepare the output span for the internal helper.
                std::span<index> neighbor_span = neighbor_buffer;

                // --- THIS IS THE REAL IMPLEMENTATION ---
                // Call the internal helper to get all neighbors for the current cell.
                if (get_neighbors(cell, neighbor_span) != error_t::none)
                    return error_t::failed; // A failure in the core logic.

                // Process the neighbors found by the helper.
                for (const index neighbor: neighbor_span)
                {
                    if (visited.find(neighbor) == visited.end())
                    {
                        visited.insert(neighbor);
                        out_ring[count++] = neighbor;
                        next_ring_frontier.push_back(neighbor);
                    }
                }
            }

            current_ring_frontier = std::move(next_ring_frontier);
        }

        // Resize the final output span to the actual number of unique cells found.
        out_ring = out_ring.subspan(0u, count);
        return error_t::none;
    }

    std::size_t path_cells_size(const index start, const index end) noexcept
    {
        const auto dist = distance(start, end);
        return (dist >= 0) ? static_cast<std::size_t>(dist + 1) : 0u;
    }

    error_t path_cells(const index start, const index end, std::span<index>& out_path) noexcept
    {
        // 1. --- Input Validation and Sizing ---
        const size_t required_size = path_cells_size(start, end);
        if (required_size == 0u)
            return error_t::domain;

        if (out_path.size() < required_size)
            return error_t::buffer_too_small;

        // 2. --- Establish Local Coordinate System ---
        // Get the IJK coordinates of the end point relative to the start point.
        coordinate::ijk end_ijk;
        if (index_to_local_ijk(start, end, end_ijk) != error_t::none)
            return error_t::failed;

        // 3. --- Hexagonal Line Drawing Algorithm ---
        // This is a direct implementation of the hex line-drawing algorithm, which
        // interpolates points in cube coordinate space and rounds to the nearest center.
        const int dist = static_cast<int>(required_size - 1);
        if (dist == 0)
        { // If start and end are the same cell
            out_path[0] = start;
            out_path = out_path.subspan(0, 1u);
            return error_t::none;
        }

        // Pre-calculate fractional steps for interpolation.
        const double i_step = static_cast<double>(end_ijk.i) / dist;
        const double j_step = static_cast<double>(end_ijk.j) / dist;
        const double k_step = static_cast<double>(end_ijk.k) / dist;

        size_t count = 0;
        for (int i = 0; i <= dist; ++i)
        {
            // a. Interpolate to find the ideal fractional coordinate at this step.
            const double i_frac = i_step * i;
            const double j_frac = j_step * i;
            const double k_frac = k_step * i;

            // b. Round the fractional coordinate to the nearest integer cube coordinate center.
            coordinate::ijk::value i_round, j_round, k_round;
            cube_round(i_frac, j_frac, k_frac, i_round, j_round, k_round);
            const coordinate::ijk current_local_ijk {i_round, j_round, k_round};

            // c. Convert this local IJK coordinate back into a global GeoHex index.
            index current_cell;
            if (local_ijk_to_index(start, current_local_ijk, current_cell) != error_t::none)
                return error_t::failed;

            // d. Add to output, ensuring no duplicates (can happen due to rounding).
            if (count == 0 || out_path[count - 1] != current_cell)
                out_path[count++] = current_cell;
        }

        // 4. --- Finalize Output ---
        out_path = out_path.subspan(0, count);
        return error_t::none;
    }
}
