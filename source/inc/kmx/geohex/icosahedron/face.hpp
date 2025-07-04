/// @file inc/kmx/geohex/icosahedron/face.hpp
/// @ingroup Internal
/// @brief Defines types and functions for managing the 20 faces of icosahedron.
#pragma once
#ifndef PCH
    #include <kmx/geohex/base.hpp>
    #include <kmx/geohex/cell/pentagon.hpp>
    #include <kmx/geohex/coordinate/ijk.hpp>
    #include <kmx/geohex/index.hpp>
    #include <kmx/gis/wgs84/coordinate.hpp>
    #include <kmx/math/vector.hpp>
    #include <optional>
#endif

namespace kmx::geohex
{
    /// @brief Calculates the squared Euclidean distance between two 2D hexagon centers.
    /// @details A performance-oriented utility that avoids a square root, suitable for
    ///          distance comparisons.
    /// @ref _hex2dDist
    template <typename T>
    constexpr T hex2d_distance_sq(const math::vector2<T>& v1, const math::vector2<T>& v2) noexcept
    {
        const T dx = v1.x - v2.x;
        const T dy = v1.y - v2.y;
        return dx * dx + dy * dy;
    }

    /// @brief Calculates the Euclidean distance between two 2D hexagon centers.
    /// @ref _hex2dDist
    template <typename T>
    constexpr T hex2d_distance(const math::vector2<T>& v1, const math::vector2<T>& v2) noexcept
    {
        return std::sqrt(hex2d_distance_sq(v1, v2));
    }
}

namespace kmx::geohex::icosahedron::face
{
    /// @brief A type alias for an icosahedron face number (0-19).
    using no_t = std::uint8_t;

    /// @brief An enumeration of the 20 icosahedron faces, from 0 to 19.
    enum class id_t : no_t
    {
        f0,
        f1,
        f2,
        f3,
        f4,
        f5,
        f6,
        f7,
        f8,
        f9,
        f10,
        f11,
        f12,
        f13,
        f14,
        f15,
        f16,
        f17,
        f18,
        f19,
    };

    /// @brief The total number of icosahedron faces.
    /// @ref NUM_ICOSA_FACES
    constexpr no_t count = +id_t::f19 + 1u;

    /// @brief Determines which icosahedron face a given base cell primarily resides on.
    /// @param base_cell_id The ID of the base cell (0-121).
    /// @return The ID of the icosahedron face.
    id_t of(const cell::base::id_t base_cell_id) noexcept;

    /// @brief Checks if a face is a clockwise offset from a pentagonal base cell.
    /// @details Used in pentagon distortion logic to determine rotational adjustments.
    /// @param base_cell_id The ID of the pentagonal base cell.
    /// @param face The ID of the face to check.
    /// @return `true` if the face is a clockwise-offset neighbor of the pentagon.
    bool is_cw_offset(const cell::base::id_t base_cell_id, const id_t face) noexcept;

    /// @brief Finds all icosahedron faces that a given H3 cell's boundary intersects.
    /// @details A cell can span across the edges of multiple icosahedron faces, especially
    ///          at coarser resolutions or near icosahedron vertices (pentagons).
    /// @ref getIcosahedronFaces
    /// @param index The H3 cell index.
    /// @param[out] output A span to be filled with the unique face IDs. The span will be
    ///                    resized to the actual number of faces found.
    /// @return `error_t::none` on success.
    error_t get_intersected(const index index, std::span<no_t>& output) noexcept;

    /// @brief A type alias for a 3D vector, used for point representations.
    using vector3 = math::vector3d;

    /// @brief Retrieves the 3D Cartesian coordinates of an icosahedron face's center.
    /// @param face The ID of the icosahedron face.
    /// @return A `vector3` representing the center point on the unit sphere.
    vector3 center_point(const id_t face) noexcept;

    /// @brief Retrieves the geographic WGS84 coordinate of an icosahedron face's center.
    /// @param face The ID of the icosahedron face.
    /// @return A `gis::wgs84::coordinate` of the face center.
    gis::wgs84::coordinate center_wgs(const id_t face) noexcept;

    /// @brief Represents a coordinate on a specific icosahedron face.
    /// @details This is the core internal representation for a point in the H3 system. It
    ///          combines a face ID with IJK coordinates on that face's planar grid.
    /// @ref FaceIJK
    struct ijk
    {
        coordinate::ijk ijk_coords {}; ///< IJK coordinates on the face's grid.
        id_t face {id_t::f0};          ///< The icosahedron face ID (0-19).

        constexpr bool operator==(const ijk&) const noexcept = default;
        constexpr bool operator!=(const ijk&) const noexcept = default;
    };

    /// @brief Gets the "home" FaceIJK for a given base cell.
    /// @details The home `FaceIJK` is the canonical reference point for a base cell,
    ///          defining its position and face in the resolution-0 grid.
    /// @param base_id The ID of the base cell.
    /// @return The home `ijk` for that base cell.
    ijk home(const cell::base::id_t base_id) noexcept;

    /// @brief Represents a `FaceIJK` with an explicit orientation.
    /// @details This struct extends `ijk` to include the number of counter-clockwise
    ///          60-degree rotations applied to the IJK coordinate system. This is
    ///          essential for tracking the system's orientation during grid traversals
    ///          that cross face boundaries.
    /// @note This is analogous to H3 C's internal `FaceOrientIJK` struct.
    struct oriented_ijk: ijk
    {
        /// Number of counter-clockwise 60-degree rotations vs the face's canonical orientation.
        std::int8_t ccw_rotations_60 {};

        constexpr bool operator==(const oriented_ijk&) const noexcept = default;
        constexpr bool operator!=(const oriented_ijk&) const noexcept = default;
    };

    /// @brief Converts an H3 cell index to its corresponding `FaceIJK` representation.
    /// @details This is a fundamental conversion that "unpacks" an H3 index into the
    ///          internal coordinate system used for most algorithms.
    /// @ref _h3ToFaceIjk (H3 C internal)
    /// @param index The H3 cell index.
    /// @param[out] out The structure to fill with the `FaceIJK` representation.
    /// @return `error_t::none` on success, or an error code if the index is invalid.
    error_t from_index(const index index, ijk& out) noexcept;

    /// @brief Converts a `FaceIJK` representation back into a canonical H3 index.
    /// @details This is the inverse of `from_index`. It "packs" the internal face and
    ///          IJK coordinates into the 64-bit H3 index format by determining the
    ///          appropriate base cell and sequence of direction digits.
    /// @ref _faceIjkToH3
    /// @param fijk The `FaceIJK` to convert.
    /// @param res The resolution for the output H3 index.
    /// @param[out] out_index The resulting H3 index.
    /// @return `error_t::none` on success.
    error_t to_index(const ijk& fijk, resolution_t res, index& out_index) noexcept;

    /// @brief Determines the closest icosahedron face for a given geographic coordinate.
    /// @param coord The WGS84 coordinate.
    /// @return The ID of the nearest face.
    id_t from_wgs(const gis::wgs84::coordinate& coord) noexcept;

    /// @brief Converts `FaceIJK` coordinates to a geographic WGS84 coordinate.
    /// @details This involves projecting the point from the 2D plane of the icosahedron
    ///          face back onto the sphere.
    /// @ref _faceIjkToGeo
    /// @param fijk The `FaceIJK` to convert.
    /// @param res The resolution of the `fijk`'s grid system.
    /// @param[out] out_coord The resulting WGS84 coordinate.
    /// @return `error_t::none` on success.
    error_t to_wgs(const ijk& fijk, const resolution_t res, gis::wgs84::coordinate& out_coord) noexcept;

    /// @brief Converts geographic WGS84 coordinates to a `FaceIJK` representation.
    /// @details This is the core of the `geoToH3` algorithm. It projects the WGS84 point
    ///          onto the icosahedron, finds the best-fit face, and calculates the
    //           corresponding IJK coordinates on that face's grid.
    /// @ref _geoToFaceIjk (H3 C internal)
    /// @param coord The WGS84 geographic coordinate (in radians).
    /// @param res The target H3 resolution.
    /// @param[out] out_fijk The resulting `FaceIJK` representation.
    /// @return `error_t::none` on success.
    error_t from_wgs(const gis::wgs84::coordinate& coord, const resolution_t res, ijk& out_fijk) noexcept;

    /// @brief Retrieves the `FaceIJK` of a pentagon's neighbor in a specific direction.
    /// @details Provides a lookup for the pre-calculated neighbor data across face
    ///          boundaries around a pentagon.
    /// @param pentagon_no The local index of the pentagon (0-11).
    /// @param direction The direction of movement.
    /// @return An `std::optional` containing the neighbor `ijk` if the direction is valid
    ///         for a pentagon, `std::nullopt` otherwise.
    std::optional<ijk> get(const std::uint8_t pentagon_no, const direction_t direction) noexcept;

    /// @brief Adjusts `FaceIJK` coordinates for pentagon distortion when crossing a face boundary.
    /// @details This is the core "worker" function containing the complex logic for transforming
    ///          coordinates when moving from a pentagonal cell onto an adjacent icosahedron face.
    ///          This function does not *detect* overage; it performs the *correction* after
    ///          a higher-level algorithm has determined a face crossing occurred.
    /// @ref _adjustOverageClassII and _adjustOverageClassIII (H3 C internal)
    /// @param[in,out] current_face The current face ID, updated to the new face on success.
    /// @param[in,out] current_ccw_rotations_60 The current system's rotation, updated on success.
    /// @param[in,out] ijk_coords_to_adjust The IJK coordinates, replaced with coordinates on the new face.
    /// @param base_cell_id The ID of the pentagonal base cell where the move originated.
    /// @param digit_moved The direction of movement that caused the overage.
    /// @return `error_t::none` on success.
    error_t adjust_overage(id_t& current_face, std::int8_t& current_ccw_rotations_60, coordinate::ijk& ijk_coords_to_adjust,
                           const cell::base::id_t base_cell_id, const direction_t digit_moved) noexcept;

    /// @brief Convenience wrapper to adjust for pentagon overage using an `oriented_ijk` struct.
    /// @details This function provides a simpler API by determining the base cell from the
    ///          provided `fijk` and then delegating the complex adjustment logic to the
    ///          more detailed `adjust_overage` worker function.
    /// @param[in,out] fijk The `FaceIJK` with orientation to adjust. Its members are modified in-place.
    /// @param res The resolution of the grid system.
    /// @param digit The direction of movement that may have caused an overage.
    /// @return `error_t::none` on success, or an error propagated from the core adjustment logic.
    error_t adjust_overage(oriented_ijk& fijk, const resolution_t res, const direction_t digit) noexcept;

    /// @brief Determines the H3 base cell and resolution-0 orientation for a given `FaceIJK`.
    /// @details This is a key part of the reverse mapping from coordinates to an H3 index,
    ///          corresponding to the core logic of `_faceIjkToH3` in the H3 C library.
    /// @param fijk The `FaceIJK` coordinates to find the base cell for.
    /// @param res The resolution of the provided `fijk`.
    /// @param[out] out_base_cell The determined base cell ID (0-121).
    /// @param[out] out_orientation The canonical orientation of the base cell at resolution 0.
    /// @return `error_t::none` on success, or an error code if no base cell could be found.
    error_t to_base_cell_and_orientation(const ijk& fijk, const resolution_t res, cell::base::id_t& out_base_cell,
                                         int& out_orientation) noexcept;

    /// @brief Simplified helper to determine the H3 base cell for a given `FaceIJK`.
    /// @param fijk The `FaceIJK` coordinates.
    /// @param res The resolution of the provided `fijk`.
    /// @return The base cell ID (0-121), or `base::invalid_index` on failure.
    cell::base::id_t to_base_cell(const ijk& fijk, const resolution_t res) noexcept;

    /// @brief Converts a `FaceIJK` representation at one resolution to its IJK coordinates at another.
    /// @param fijk_higher_res The `FaceIJK` at the finer resolution.
    /// @param res_higher The finer resolution.
    /// @param res_lower The coarser resolution.
    /// @param[out] out_ijk_lower_res The resulting IJK coordinates at the coarser resolution.
    /// @return `error_t::none` on success.
    error_t to_ijk_at_resolution(const ijk& fijk_higher_res, const resolution_t res_higher, const resolution_t res_lower,
                                 coordinate::ijk& out_ijk_lower_res) noexcept;

    /// @brief Finds the `FaceIJK` of a neighboring cell, handling icosahedron face-crossing ("overage").
    /// @details This is a core internal function that orchestrates grid traversal. It corresponds to the
    ///          logic in H3's `_h3Neighbor` function. It first performs a simple IJK translation and
    ///          then checks if the new coordinates have exceeded the bounds of the current face's
    ///          coordinate system. If they have, it calls the appropriate low-level adjustment
    ///          function based on whether the origin cell was a hexagon or a pentagon.
    /// @param start_fijk The `FaceIJK` of the starting cell.
    /// @param res The H3 resolution of the grid.
    /// @param dir The direction to move to find the neighbor.
    /// @param[out] out_neighbor_fijk The resulting `FaceIJK` of the neighbor cell.
    /// @return `error_t::none` on success.
    error_t find_neighbor_face_ijk(const oriented_ijk& start_fijk, const resolution_t res, const direction_t dir,
                                   oriented_ijk& out_neighbor_fijk) noexcept;

    /// @brief Finds the local index (0, 1, or 2) of a global icosahedron vertex on a given face.
    /// @param face The face on which to find the vertex.
    /// @param vertex_to_find The global ID of the icosahedron vertex (0-11).
    /// @return The local index of the vertex on the face's corner, or -1 if not found.
    std::int8_t find_vertex_on_face(const id_t face, const std::int8_t vertex_to_find) noexcept;
}

namespace std
{
    /// @brief Hash specialization for the full FaceIJK struct.
    template <>
    struct hash<kmx::geohex::icosahedron::face::ijk>
    {
        std::size_t operator()(const kmx::geohex::icosahedron::face::ijk& fijk) const noexcept
        {
#ifdef KMX_GEOHEX_SIMPLE_HASH
            const std::size_t h1 = std::hash<kmx::geohex::coordinate::ijk> {}(fijk.ijk_coords);
            const std::size_t h2 = std::hash<kmx::geohex::icosahedron::face::no_t> {}(+fijk.face);
            return h1 ^ (h2 << 1u);
#else
            std::size_t seed {};
            kmx::hash_combine(seed, fijk.ijk_coords);
            kmx::hash_combine(seed, kmx::operator+(fijk.face));
            return seed;
#endif
        }
    };
}
