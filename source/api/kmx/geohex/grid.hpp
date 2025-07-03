/// @file api/kmx/geohex/grid.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
    #include <span>
#endif

namespace kmx::geohex::grid
{
    /// @brief Calculates the grid distance (number of cells) between two indexes.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref h3Distance
    /// @param a The first H3 index.
    /// @param b The second H3 index.
    /// @return The grid distance, or a negative value if the distance cannot be computed.
    [[nodiscard]] int distance(const index a, const index b) noexcept;

    // k-Ring Functions

    /// @brief Calculates the maximum number of cells that can be in a k-ring for a given k.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref maxKringSize
    /// @param k The grid distance (k >= 0).
    /// @return The maximum possible number of cells in the k-ring.
    [[nodiscard]] constexpr std::size_t max_k_ring_size(const int k) noexcept
    {
        if (k < 0)
            return 0u;

        // The formula for the max number of cells in a k-ring is 3k(k+1) + 1.
        return static_cast<std::size_t>(3u) * k * (k + 1u) + 1u;
    }

    /// @brief Finds all cells within a grid distance `k` of an origin cell.
    /// @details This is a non-allocating function that is guaranteed not to throw exceptions.
    ///          The caller must provide a span large enough to hold the result, determined
    ///          by `max_k_ring_size()`. On success, the output span is resized to the
    ///          actual number of cells found.
    /// @ref kRing
    /// @param origin The origin H3 index.
    /// @param k The grid distance (k >= 0).
    /// @param[out] out_ring A span to be filled with the k-ring indexes.
    /// @return `error_t::none` on success, or `error_t::buffer_too_small`.
    error_t k_ring(const index origin, const int k, std::span<index>& out_ring) noexcept;

    // Path Functions

    /// @brief Calculates the exact number of cells in the path between two indexes.
    /// @details The size of the path is `grid_distance + 1`. This function is guaranteed
    ///          not to throw exceptions.
    /// @param start The starting H3 index.
    /// @param end The ending H3 index.
    /// @return The exact number of cells in the path, or 0 on error.
    [[nodiscard]] std::size_t path_cells_size(const index start, const index end) noexcept;

    /// @brief Finds the line of cells connecting two indexes.
    /// @details This is a non-allocating function that is guaranteed not to throw exceptions.
    ///          The caller must provide a span large enough to hold the result, determined
    ///          by `path_cells_size()`. On success, the output span is resized to the
    ///          actual number of cells found.
    /// @ref h3Line
    /// @param start The starting H3 index.
    /// @param end The ending H3-index.
    /// @param[out] out_path A span to be filled with the path indexes.
    /// @return `error_t::none` on success, or `error_t::buffer_too_small`.
    error_t path_cells(const index start, const index end, std::span<index>& out_path) noexcept;

} // namespace kmx::geohex::grid
