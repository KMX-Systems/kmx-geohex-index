/// @file api/kmx/geohex/region.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
    #include <kmx/gis/wgs84/coordinate.hpp>
    #include <span>
    #include <vector>
#endif

namespace kmx::geohex::region
{
    /// @brief An internal helper to perform a point-in-polygon test.
    /// @details Implements the winding number algorithm (ray casting) to determine if a
    ///          geographic point is inside a polygon. Handles polygons that cross the
    ///          antimeridian.
    /// @param point The point to test.
    /// @param polygon The vertices of the polygon.
    /// @return True if the point is inside the polygon, false otherwise.
    bool is_inside_polygon(const gis::wgs84::coordinate& point, std::span<const gis::wgs84::coordinate> polygon) noexcept;

    // Polyfill Functions

    /// @brief Calculates an upper bound on the number of cells needed to fill a polygon.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref maxPolyfillSize
    /// @param polygon A read-only span of WGS84 coordinates defining the polygon boundary.
    /// @param res The target resolution.
    /// @return The estimated maximum number of cells.
    [[nodiscard]] std::size_t max_polyfill_size(std::span<const gis::wgs84::coordinate> polygon, const resolution_t res) noexcept;

    /// @brief Fills a geographic polygon with cells (allocating version).
    /// @details This function returns a `std::vector` and may allocate memory, which can
    ///          throw `std::bad_alloc`. It is NOT marked `noexcept`.
    /// @ref polyfill
    [[nodiscard]] std::vector<index> polyfill(std::span<const gis::wgs84::coordinate> polygon, const resolution_t res);

    /// @brief Fills a geographic polygon with cells (non-allocating version).
    /// @details This function is guaranteed not to throw exceptions. Use `max_polyfill_size`
    ///          to determine the required buffer size for `out_cells`.
    /// @ref polyfill
    /// @param[out] out_cells A span to be filled with the resulting indexes. The span
    ///                       will be resized to the actual number of cells written.
    /// @return `error_t::none` on success.
    error_t polyfill(std::span<const gis::wgs84::coordinate> polygon, const resolution_t res, std::span<index>& out_cells) noexcept;

    // Compact/Uncompact Functions

    /// @brief Calculates the number of cells that will result from uncompacting a set.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref uncompactCellsSize
    /// @param compacted_cells A read-only span of the compacted set of indexes.
    /// @param res The target resolution to uncompact to.
    /// @return The exact number of cells in the uncompacted set, or 0 on error.
    [[nodiscard]] std::size_t uncompact_size(std::span<const index> compacted_cells, const resolution_t res) noexcept;

    /// @brief Decompresses a set of cells to a specific resolution (non-allocating).
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref uncompactCells
    /// @param[out] out_cells A span to be filled with the uncompacted set. Use `uncompact_size`
    ///                       to determine the required buffer size.
    /// @return `error_t::none` on success.
    error_t uncompact(std::span<const index> compacted_cells, const resolution_t res, std::span<index>& out_cells) noexcept;

    /// @brief Calculates the maximum workspace size needed for the `compact` function.
    /// @details The `compact` algorithm requires temporary storage to hold unique parent
    ///          candidates at each resolution. The maximum number of such candidates
    ///          occurs when every cell in the input has a unique parent, so the
    ///          workspace size is bounded by the input size.
    /// @param num_cells The number of cells in the input set to be compacted.
    /// @return The required size for the temporary workspace buffer.
    [[nodiscard]] constexpr std::size_t compact_workspace_size(const std::size_t num_cells) noexcept
    {
        return num_cells;
    }

    /// @brief Compacts a set of cells into a smaller, equivalent set (fully non-allocating version).
    /// @details This function is guaranteed not to throw exceptions and performs no dynamic
    ///          memory allocations. It requires the caller to provide both a destination
    ///          buffer and a temporary workspace buffer.
    /// @ref compactCells
    /// @param cells A read-only span of indexes to compact.
    /// @param[out] out_compacted A span to be filled with the compacted set. Its size must be
    ///                         at least `cells.size()`. It will be resized to the actual output size.
    /// @param[in,out] workspace A mutable span for temporary calculations. Its size must be
    //                           at least `compact_workspace_size(cells.size())`.
    /// @return `error_t::none` on success.
    error_t compact(std::span<const index> cells, std::span<index>& out_compacted, std::span<index> workspace) noexcept;

    /// @brief Compacts a set of cells, populating a user-provided vector.
    /// @details This is a highly efficient convenience wrapper for the core non-allocating
    ///          `compact` algorithm. It intelligently reuses the output vector's capacity
    ///          to serve as temporary workspace, minimizing or eliminating dynamic memory
    ///          allocations in repeated calls. The function is guaranteed not to throw,
    ///          converting potential memory allocation failures into an error code.
    /// @param cells A read-only span of indexes to be compacted.
    /// @param[out] out_compacted A vector that will be cleared and then filled with the
    ///                         compacted result. Its capacity may be increased if insufficient.
    /// @return `error_t::none` on success, `error_t::memory_alloc` on allocation failure.
    error_t compact(std::span<const index> cells, std::vector<index>& out_compacted) noexcept;

} // namespace kmx::geohex::region
