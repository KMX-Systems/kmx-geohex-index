/// @file api/kmx/geohex/vertex.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
    #include <kmx/gis/wgs84/coordinate.hpp>
    #include <span>
#endif

namespace kmx::geohex::vertex
{
    /// @brief Gets the index of a specific vertex of a cell.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref cellToVertex
    /// @param cell The H3 cell index.
    /// @param vertex_num The vertex number to get (0-5).
    /// @return The H3 vertex index.
    [[nodiscard]] index from_cell(const index cell, const std::uint8_t vertex_num) noexcept;

    /// @brief Gets all vertex indexes for a cell.
    /// @details This is a non-allocating function that is guaranteed not to throw exceptions.
    /// @ref cellToVertexes
    /// @param cell The H3 cell index.
    /// @param[out] out_vertices A span to be filled with the H3 vertex indexes. Its size must
    ///                        be at least 6. The span will be resized to the actual count.
    /// @return `error_t::none` on success.
    error_t from_cell(const index cell, std::span<index>& out_vertices) noexcept;

    /// @brief Gets the geographic coordinate of a vertex index.
    /// @details This function is guaranteed not to throw exceptions.
    /// @ref vertexToGeo
    /// @param vertex The H3 vertex index.
    /// @param[out] out_coord The structure to fill with the vertex coordinate.
    /// @return `error_t::none` on success.
    error_t to_wgs(const index vertex, gis::wgs84::coordinate& out_coord) noexcept;

} // namespace kmx::geohex::vertex
