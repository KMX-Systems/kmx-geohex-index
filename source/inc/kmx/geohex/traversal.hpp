/// @file inc/kmx/geohex/index_utils.hpp
/// @ingroup Internal
/// @brief Core enumerations, type aliases, and fundamental utilities for the GeoHex library.
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
#endif

namespace kmx::geohex
{
    namespace coordinate
    {
        class ijk;
    }

    /// @brief Calculates the indexes of all valid neighbors of a given cell.
    /// @details
    /// This is the core internal worker for all neighbor-finding operations. It provides a
    /// robust and canonical way to perform a single-step grid traversal.
    ///
    /// The algorithm follows these steps:
    /// 1. Converts the `origin` index to its internal, oriented `FaceIJK` representation.
    /// 2. Iterates through the six valid neighbor directions (K through IJ).
    /// 3. For each direction, it calls `icosahedron::face::find_neighbor_face_ijk`, which
    ///    delegates the complex logic of IJK math and icosahedron face-crossing.
    /// 4. The resulting neighbor `FaceIJK` is converted back into a global `index`.
    /// 5. The final collection of valid neighbor indexes is placed into the output span.
    ///
    /// @note
    /// This function correctly handles pentagons. When the origin is a pentagon, it will
    /// successfully find the 5 valid neighbors and the traversal in the "missing"
    /// direction will be correctly ignored, resulting in an output span of size 5.
    /// The caller must always provide a buffer of size 6, as this is the maximum
    /// possible number of neighbors.
    ///
    /// @param origin The index of the cell whose neighbors are to be found.
    /// @param[out] out_neighbors A span to be filled with the neighbor indexes. This span
    ///                         **must** have a size of at least 6 to guarantee safety.
    ///                         On success, it will be resized to the actual number of
    ///                         neighbors found (6 for a hexagon, 5 for a pentagon).
    /// @return `error_t::none` on success, `error_t::buffer_too_small` if the output
    ///         span is insufficient, or another error code if the core traversal
    ///         logic fails.
    ///
    /// @see grid::k_ring, unidirectional_edge::are_neighbors, icosahedron::face::find_neighbor_face_ijk
    error_t get_neighbors(const index origin, std::span<index>& out_neighbors) noexcept;

    /// @brief Calculates the local IJK coordinates of an index relative to an origin index.
    /// @details This is the core worker function for distance and line-drawing algorithms.
    ///          It establishes a local coordinate system with the `origin` index at its
    ///          center {0,0,0} and finds the coordinates of the `target` index within
    ///          that system. This may involve a search over the base cell graph to
    ///          unify indexes on different icosahedron faces.
    /// @param origin The index that defines the origin of the local IJK system.
    /// @param target The index to find the coordinates of.
    /// @param[out] out_ijk The resulting IJK coordinates of `target` in the local system.
    /// @return `error_t::none` on success, or an error code on failure.
    error_t index_to_local_ijk(const index& origin, const index& target, coordinate::ijk& out_ijk) noexcept;

    /// @brief Converts a local IJK coordinate relative to an origin index back to a global index.
    /// @details This is the inverse of `index_to_local_ijk`. It is a complex operation that
    ///          reconstructs a global index from a local coordinate by applying the same
    ///          transformations used to create the local system, but in reverse. It is
    ///          essential for algorithms like grid line-drawing.
    /// @param origin The index that defines the origin {0,0,0} of the local IJK system.
    /// @param ijk The local IJK coordinates of the target cell.
    /// @param[out] out_index The resulting global GeoHex index.
    /// @return `error_t::none` on success.
    error_t local_ijk_to_index(const index origin, const coordinate::ijk& ijk, index& out_index) noexcept;
}
