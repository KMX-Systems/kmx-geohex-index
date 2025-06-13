/// @file api/kmx/geohex/unidirectional_edge.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
    #include <kmx/gis/wgs84/coordinate.hpp>
    #include <span>
#endif

namespace kmx::geohex::unidirectional_edge
{
    /// @brief Checks if two cells are immediate neighbors.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref areNeighbors
    [[nodiscard]] bool are_neighbors(const index a, const index b) noexcept;

    /// @brief Creates a directed edge index from an origin cell to a destination neighbor.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref cellsToDirectedEdge
    /// @return The directed edge index, or an invalid index on error.
    [[nodiscard]] index from_cells(const index origin, const index destination) noexcept;

    /// @brief Gets the origin cell of a directed edge.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref getDirectedEdgeOrigin
    [[nodiscard]] index origin(const index edge) noexcept;

    /// @brief Gets the destination cell of a directed edge.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref getDirectedEdgeDestination
    [[nodiscard]] index destination(const index edge) noexcept;

    /// @brief Gets the geographic boundary of a directed edge.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref directedEdgeToBoundary
    /// @param[out] out_boundary A span to be filled with the boundary vertices. Its size
    ///                        must be at least 4. The span will be resized to the actual count.
    /// @return `error_t::none` on success.
    error_t get_boundary(const index edge, std::span<gis::wgs84::coordinate>& out_boundary) noexcept;

} // namespace kmx::geohex::unidirectional_edge
