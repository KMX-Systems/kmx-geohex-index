/// @file src/kmx/geohex/region.cpp
#include "kmx/geohex/region.hpp"
#include "kmx/geohex/grid.hpp"
#include "kmx/geohex/traversal.hpp"
#include <algorithm>
#include <queue>
#include <unordered_set>

namespace kmx::geohex::region
{
    bool is_inside_polygon(const gis::wgs84::coordinate& point, std::span<const gis::wgs84::coordinate> polygon) noexcept
    {
        if (polygon.size() < 3u)
            return false;

        int crossings {};
        for (std::size_t i {}; i < polygon.size(); ++i)
        {
            const gis::wgs84::coordinate& p1 = polygon[i];
            const gis::wgs84::coordinate& p2 = polygon[(i + 1u) % polygon.size()];

            // Ensure p1 is the vertex with the smaller latitude
            const gis::wgs84::coordinate& a = (p1.latitude < p2.latitude) ? p1 : p2;
            const gis::wgs84::coordinate& b = (p1.latitude < p2.latitude) ? p2 : p1;

            if (point.latitude <= a.latitude || point.latitude > b.latitude)
                continue; // Point is not between the y-coordinates of the edge

            // Handle the antimeridian by checking the shortest longitude distance
            double val =
                (b.longitude - a.longitude) * (point.latitude - a.latitude) - (b.latitude - a.latitude) * (point.longitude - a.longitude);

            if (std::abs(b.longitude - a.longitude) > std::numbers::pi)
            { // Edge crosses antimeridian
                val = (b.longitude > a.longitude) ? (b.longitude - a.longitude - 2 * std::numbers::pi) * (point.latitude - a.latitude) -
                                                        (b.latitude - a.latitude) * (point.longitude - a.longitude) :
                                                    (b.longitude - a.longitude + 2 * std::numbers::pi) * (point.latitude - a.latitude) -
                                                        (b.latitude - a.latitude) * (point.longitude - a.longitude);
            }

            if (val > 0) // Point is to the left of the edge, counts as a crossing
                ++crossings;
        }

        return (crossings % 2) == 1;
    }

    std::size_t max_polyfill_size(std::span<const gis::wgs84::coordinate> polygon, const resolution_t res) noexcept
    {
        if (polygon.empty())
            return 0u;

        // 1. Calculate the bounding box of the polygon.
        double min_lat = std::numeric_limits<double>::max();
        double max_lat = std::numeric_limits<double>::lowest();
        double min_lon = std::numeric_limits<double>::max();
        double max_lon = std::numeric_limits<double>::lowest();

        for (const auto& p: polygon)
        {
            min_lat = std::min(min_lat, p.latitude);
            max_lat = std::max(max_lat, p.latitude);
            min_lon = std::min(min_lon, p.longitude);
            max_lon = std::max(max_lon, p.longitude);
        }

        // 2. Get the corners of the bounding box.
        const index top_left = index::from_wgs({max_lat, min_lon}, res);
        const index top_right = index::from_wgs({max_lat, max_lon}, res);
        const index bottom_left = index::from_wgs({min_lat, min_lon}, res);

        // 3. Estimate the number of cells by the size of the bounding box in grid space.
        const auto width = static_cast<std::size_t>(grid::distance(top_left, top_right));
        const auto height = static_cast<std::size_t>(grid::distance(top_left, bottom_left));

        // 4. The number of cells is roughly the area of the bounding parallelogram in the grid.
        //    Add a safety margin (e.g., +2 on each dimension and a fixed buffer) to account for
        //    irregular shapes and grid distortion.
        const std::size_t estimate = (width + 2u) * (height + 2u) + 12u; // 12 is a generic safety buffer

        // Add the size of the polygon trace itself as a baseline.
        std::size_t trace_size {};
        for (std::size_t i {}; i < polygon.size(); ++i)
        {
            const index p1_idx = index::from_wgs(polygon[i], res);
            const index p2_idx = index::from_wgs(polygon[(i + 1) % polygon.size()], res);
            trace_size += grid::path_cells_size(p1_idx, p2_idx);
        }

        return estimate + trace_size;
    }

    std::vector<index> polyfill(std::span<const gis::wgs84::coordinate> polygon, const resolution_t res)
    {
        const std::size_t required_size = max_polyfill_size(polygon, res);
        std::vector<index> result_vec(required_size);
        std::span<index> out_span = result_vec;

        if (polyfill(polygon, res, out_span) == error_t::none)
        {
            result_vec.resize(out_span.size()); // Shrink to fit the actual number of cells
            return result_vec;
        }

        return {}; // Return empty vector on error
    }

    error_t polyfill(std::span<const gis::wgs84::coordinate> polygon, const resolution_t res, std::span<index>& out_cells) noexcept
    {
        // 1. Input Validation
        if (polygon.size() < 3u)
            return error_t::domain;

        const std::size_t max_size = max_polyfill_size(polygon, res);
        if (out_cells.size() < max_size)
            return error_t::buffer_too_small;

        // Use a hash set for efficient O(1) insertion and duplicate handling.
        std::unordered_set<index> found_cells;

        // 2. Trace Polygon Edges
        // Walk along each segment of the polygon and find all grid cells that it intersects.
        for (std::size_t i {}; i < polygon.size(); ++i)
        {
            const auto p1 = polygon[i];
            const auto p2 = polygon[(i + 1u) % polygon.size()];
            const index p1_idx = index::from_wgs(p1, res);
            const index p2_idx = index::from_wgs(p2, res);

            const std::size_t path_size = grid::path_cells_size(p1_idx, p2_idx);
            if (path_size > 0u)
            {
                std::vector<index> path_buffer(path_size);
                std::span<index> path_span = path_buffer;
                if (grid::path_cells(p1_idx, p2_idx, path_span) == error_t::none)
                {
                    found_cells.insert(path_span.begin(), path_span.end());
                }
            }
        }

        // 3. Find an Interior Seed Cell
        // We need one guaranteed cell inside the polygon to start our fill from.
        // The most robust method is to check the neighbors of the traced boundary.
        index seed_cell;
        bool seed_found {};
        std::array<index, 6u> neighbor_buffer;

        for (const index boundary_cell: found_cells)
        {
            std::span<index> neighbor_span = neighbor_buffer;
            // This requires an internal `get_neighbors` function.
            if (get_neighbors(boundary_cell, neighbor_span) == error_t::none)
            {
                for (const index neighbor: neighbor_span)
                {
                    // If this neighbor isn't part of the boundary itself...
                    if (found_cells.find(neighbor) == found_cells.end())
                    {
                        gis::wgs84::coordinate center;
                        if (neighbor.to_wgs(center) == error_t::none)
                        {
                            // ...and its center is inside the polygon, we have found our seed.
                            if (is_inside_polygon(center, polygon))
                            {
                                seed_cell = neighbor;
                                seed_found = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (seed_found)
                break;
        }

        // 4. Fill Interior with Breadth-First Search (BFS)
        if (seed_found)
        {
            std::queue<index> q;
            q.push(seed_cell);
            found_cells.insert(seed_cell);

            while (!q.empty())
            {
                const index current_cell = q.front();
                q.pop();

                std::span<index> neighbor_span = neighbor_buffer;
                if (get_neighbors(current_cell, neighbor_span) == error_t::none)
                {
                    for (const index neighbor: neighbor_span)
                    {
                        if (found_cells.find(neighbor) == found_cells.end())
                        {
                            found_cells.insert(neighbor);
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
        // If no seed was found, it means the polygon is too small to have an interior,
        // and the boundary trace is the complete result.

        // 5. Finalize Output
        if (found_cells.size() > out_cells.size())
            return error_t::buffer_too_small; // Should be caught by initial check, but as a safeguard.

        std::copy(found_cells.begin(), found_cells.end(), out_cells.begin());
        out_cells = out_cells.subspan(0u, found_cells.size());

        return error_t::none;
    }

    std::size_t uncompact_size(std::span<const index> compacted_cells, const resolution_t res) noexcept
    {
        std::size_t count {};
        for (const auto& cell: compacted_cells)
        {
            if (!cell.is_valid() || cell.resolution() > res)
                return 0u; // Invalid input

            count += cell.children_count(res);
        }

        return count;
    }

    error_t uncompact(std::span<const index> compacted_cells, const resolution_t res, std::span<index>& out_cells) noexcept
    {
        const std::size_t required_size = uncompact_size(compacted_cells, res);
        if ((required_size == 0u) && !compacted_cells.empty())
            return error_t::domain;

        if (out_cells.size() < required_size)
            return error_t::buffer_too_small;

        std::size_t write_pos {};
        for (const auto& cell: compacted_cells)
        {
            std::span<index> child_span = out_cells.subspan(write_pos);
            const error_t err = cell.get_children(res, child_span);
            if (err != error_t::none)
                // This shouldn't happen if our size checks are correct.
                return err;

            write_pos += child_span.size();
        }

        out_cells = out_cells.subspan(0u, write_pos);
        return error_t::none;
    }

    /// @brief A non-allocating helper to collect unique parent candidates into a workspace.
    /// @return A subspan of the workspace containing the unique, sorted parent candidates.
    std::span<index> collect_parent_candidates_noalloc(std::span<const index> current_cells, const resolution_t res,
                                                       std::span<index> workspace)
    {
        if (res == resolution_t::r0)
            return workspace.subspan(0u, 0u);

        const resolution_t parent_res = static_cast<resolution_t>(+res - 1);
        std::size_t parent_count {};

        for (const index cell: current_cells)
            if (cell.resolution() == res)
                // This check is a safeguard, but should pass if the workspace is sized correctly.
                if (parent_count < workspace.size())
                    workspace[parent_count++] = cell.get_parent(parent_res);

        auto parents_subspan = workspace.subspan(0u, parent_count);

        // Sort the candidates to bring duplicates together.
        std::sort(parents_subspan.begin(), parents_subspan.end());

        // Erase duplicates in-place. `std::unique` moves unique elements to the front
        // and returns an iterator to the end of the unique range.
        auto last = std::unique(parents_subspan.begin(), parents_subspan.end());

        // Return a new span covering only the unique elements.
        return parents_subspan.subspan(0u, std::distance(parents_subspan.begin(), last));
    }

    error_t compact(std::span<const index> cells, std::span<index>& out_compacted, std::span<index> workspace) noexcept
    {
        // 1. Input and Buffer Validation
        if (cells.empty())
        {
            out_compacted = out_compacted.subspan(0u, 0u);
            return error_t::none;
        }

        // The compacted set can never be larger than the original set.
        if (out_compacted.size() < cells.size())
            return error_t::buffer_too_small;

        // The workspace must be large enough to hold all potential parent candidates.
        if (workspace.size() < compact_workspace_size(cells.size()))
            return error_t::buffer_too_small;

        // 2. Setup
        // Start by copying the input cells into the output buffer. We will perform
        // the compaction in-place within this buffer.
        std::copy(cells.begin(), cells.end(), out_compacted.begin());
        auto current_work_span = out_compacted.subspan(0u, cells.size());

        resolution_t max_res = resolution_t::r0;
        for (const index cell: current_work_span)
            if (cell.resolution() > max_res)
                max_res = cell.resolution();

        std::array<index, 7u> children_buffer;

        // 3. Main Compaction Loop
        for (int r = +max_res; r > 0; --r)
        {
            resolution_t current_res = static_cast<resolution_t>(r);

            // a. Get a sorted, unique list of parent candidates in our workspace.
            auto parent_candidates = collect_parent_candidates_noalloc(current_work_span, current_res, workspace);

            for (const index parent: parent_candidates)
            {
                // b. Check if this parent can be compacted.
                const auto num_children = parent.children_count(current_res);
                std::span<index> children_span = children_buffer;

                // We need a subspan of the correct size to pass to get_children.
                if (children_span.size() < num_children)
                    continue; // Safeguard
                children_span = children_span.subspan(0u, num_children);

                if (parent.get_children(current_res, children_span) != error_t::none)
                    continue;

                // c. To check if all children exist in our set efficiently, we sort both
                //    the children and our current working set and do a linear scan.
                std::sort(current_work_span.begin(), current_work_span.end());

                std::size_t found_count {};
                auto work_it = current_work_span.begin();
                for (const index child_to_find: children_span)
                {
                    // Advance the iterator in the main set until we find or pass the child.
                    work_it = std::lower_bound(work_it, current_work_span.end(), child_to_find);
                    if (work_it != current_work_span.end() && *work_it == child_to_find)
                        found_count++;
                }

                // d. If all children were found, perform the compaction.
                if (found_count == num_children)
                {
                    // Use `std::remove_if` to efficiently remove all children from the work span
                    // by shifting non-children elements to the front.
                    auto new_end = std::remove_if(current_work_span.begin(), current_work_span.end(),
                                                  [&](const index& cell)
                                                  {
                                                      // Check if `cell` is one of the children to be removed.
                                                      return std::binary_search(children_span.begin(), children_span.end(), cell);
                                                  });

                    // Resize the span to reflect the removed elements.
                    current_work_span = current_work_span.subspan(0u, std::distance(current_work_span.begin(), new_end));

                    // Add the parent cell to the end. The buffer has capacity.
                    current_work_span[current_work_span.size()] = parent;
                    current_work_span = out_compacted.subspan(0u, current_work_span.size() + 1u);
                }
            }
        }

        // 4. Finalize Output
        out_compacted = current_work_span;
        return error_t::none;
    }

    error_t compact(std::span<const index> cells, std::vector<index>& out_compacted) noexcept
    {
        // 1. Handle Empty Input
        if (cells.empty())
        {
            out_compacted.clear();
            return error_t::none;
        }

        // 2. Calculate Required Buffer Sizes
        const std::size_t num_cells = cells.size();

        // The core algorithm needs space for its output (at most `num_cells`)
        // and for its temporary workspace (also `num_cells`).
        const std::size_t required_workspace_size = compact_workspace_size(num_cells);
        const std::size_t required_total_capacity = num_cells + required_workspace_size;

        // 3. Ensure Sufficient Vector Capacity (The only `noexcept`-critical part)
        if (out_compacted.capacity() < required_total_capacity)
        {
            // The vector's capacity is insufficient. We must try to reserve more.
            // This is the only operation that can throw std::bad_alloc.
            try
            {
                // Reserve enough space for BOTH the output and the workspace.
                out_compacted.reserve(required_total_capacity);
            }
            catch (const std::bad_alloc&)
            {
                // If allocation fails, we return a specific error code.
                return error_t::memory_alloc;
            }
        }
        // From this point forward, no more allocations will occur.

        // 4. Prepare Buffers from the Vector's Storage
        // We now have a guarantee that `out_compacted.data()` points to a buffer
        // that is large enough for the entire operation.

        // Clear the vector's logical size but retain its capacity.
        out_compacted.clear();

        // Create a span for the final output, pointing to the start of the vector's buffer.
        // Its size is `num_cells` because the compacted result cannot be larger.
        std::span<index> output_span(out_compacted.data(), num_cells);

        // Create a span for the workspace, pointing to the area immediately after the output area.
        std::span<index> workspace_span(out_compacted.data() + num_cells, required_workspace_size);

        // 5. Execute the Core, Non-Allocating Algorithm
        const error_t err = compact(cells, output_span, workspace_span);

        // 6. Finalize the Vector
        if (err == error_t::none)
        {
            // The core function wrote the compacted result into the start of our buffer
            // and resized `output_span` to the exact final count.
            // We now set the vector's logical size to match what was written.
            // This operation does not reallocate memory because the capacity is sufficient.
            out_compacted.resize(output_span.size());
        }

        // If an error occurred, the vector remains empty (from the `clear()` call),
        // which is a safe and predictable state.
        return err;
    }
}
