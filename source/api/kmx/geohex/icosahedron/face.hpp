/// @file geohex/icosahedron/face.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/cell/pentagon.hpp>
    #include <kmx/geohex/coordinate/ijk.hpp>
    #include <kmx/geohex/index.hpp>
    #include <kmx/gis/wgs84/coordinate.hpp>
    #include <kmx/math/vector.hpp>
    #include <optional>
#endif

namespace kmx::geohex
{
    /// @ref _hex2dDist
    template <typename T>
    constexpr T hex2d_distance_sq(const math::vector2<T>& v1, const math::vector2<T>& v2) noexcept
    {
        const T dx = v1.x - v2.x;
        const T dy = v1.y - v2.y;
        return dx * dx + dy * dy;
    }

    /// @ref _hex2dDist
    template <typename T>
    constexpr T hex2d_distance(const math::vector2<T>& v1, const math::vector2<T>& v2) noexcept
    {
        return std::sqrt(hex2d_distance_sq(v1, v2));
    }
}

namespace kmx::geohex::icosahedron::face
{
    using no_t = std::uint8_t;

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

    /// @ref NUM_ICOSA_FACES
    constexpr no_t count = +id_t::f19 + 1u;

    id_t of(const cell::base::id_t base_cell_id);

    bool is_cw_offset(const cell::base::id_t base_cell_id, const id_t face) noexcept;

    /// @ref getIcosahedronFaces
    error_t get_intersected(const index index, std::span<no_t>& output) noexcept;

    using vector3 = math::vector3d;

    vector3 center_point(const id_t face) noexcept;

    gis::wgs84::coordinate center_wgs(const id_t face) noexcept;

    /// @brief Represents a coordinate on a specific icosahedron face.
    /// @ref FaceIJK
    /// @note This is analogous to H3 C's internal `FaceIJK` struct.
    struct ijk
    {
        coordinate::ijk ijk_coords {}; ///< IJK coordinates on that face.
        id_t face {id_t::f0};          ///< The icosahedron face.

        constexpr bool operator==(const ijk&) const noexcept = default;
        constexpr bool operator!=(const ijk&) const noexcept = default;
    };

    ijk home(const cell::base::id_t base_id);

    /// @brief Represents a coordinate on a specific icosahedron face with orientation.
    /// @note This is analogous to H3 C's internal `FaceOrientIJK` struct.
    struct oriented_ijk: ijk
    {
        std::int8_t ccw_rotations_60 {}; ///< Counter-clockwise 60 deg rotations vs face canonical.

        constexpr bool operator==(const oriented_ijk&) const noexcept = default;
        constexpr bool operator!=(const oriented_ijk&) const noexcept = default;
    };

    /// @ref _h3ToFaceIjk (H3 C internal)
    /// @brief Converts an H3 cell index to its corresponding FaceIJK representation.
    /// @param index The H3 cell index.
    /// @param out Output: The FaceIJK representation.
    /// @return error_t::none on success, or an error code.
    error_t from_index(const index index, ijk& out) noexcept;

    /// @brief Converts a FaceIJK representation back into a canonical H3 index.
    /// @details This is the logical inverse of `from_index`.
    /// @ref _faceIjkToH3
    error_t to_index(const ijk& fijk, resolution_t res, index& out_index) noexcept;

    id_t from_wgs(const gis::wgs84::coordinate& coord) noexcept;

    /// @brief Converts FaceIJK coordinates to a geographic WGS84 coordinate.
    /// @ref _faceIjkToGeo
    error_t to_wgs(const ijk& fijk, const resolution_t res, gis::wgs84::coordinate& out_coord) noexcept;

    /// @ref _geoToFaceIjk (H3 C internal)
    /// @brief Converts geographic WGS84 coordinates (radians) to FaceIJK representation.
    error_t from_wgs(const gis::wgs84::coordinate& coord, const resolution_t res, ijk& out_fijk) noexcept;

    std::optional<ijk> get(const std::uint8_t pentagon_no, const direction_t direction) noexcept;

    /// @brief Adjusts FaceIJK coordinates for pentagon distortion when crossing icosahedron face boundaries.
    /// @details This is the core "worker" function that contains the complex logic for handling the
    ///          coordinate system transformations that occur when moving from a pentagonal base cell
    ///          onto an adjacent icosahedron face. It is the C++ equivalent of H3's internal
    ///          `_adjustOverageClassII` and `_adjustOverageClassIII` functions.
    /// @note    This function does not *detect* overage; it performs the *correction* after a
    ///          higher-level algorithm has already determined that a traversal has crossed a face
    ///          boundary from a pentagon.
    /// @param[in,out] current_face The current icosahedron face. On success, this is updated to the new face ID.
    /// @param[in,out] current_ccw_rotations_60 The current system's counter-clockwise 60-degree rotations.
    ///                                         This is updated if the new face has a different orientation.
    /// @param[in,out] ijk_coords_to_adjust The IJK coordinates to adjust. On success, these are replaced
    ///                                     with the corresponding coordinates on the new face's system.

    /// @param[in] base_cell_id The ID of the pentagonal base cell from which the move originated.
    /// @param[in] digit_moved The direction of movement that caused the overage and triggered this adjustment.
    /// @return error_t::none on success, or an error code if adjustment fails (e.g., invalid input).
    error_t adjust_overage(id_t& current_face, std::int8_t& current_ccw_rotations_60, coordinate::ijk& ijk_coords_to_adjust,
                           const cell::base::id_t base_cell_id, const direction_t digit_moved) noexcept;

    /// @brief Convenience wrapper to adjust for pentagon overage using a bundled `oriented_ijk` struct.
    /// @details This function provides a simpler public-facing API for the most common use case.
    ///          It determines the base cell from the provided `fijk` and then delegates the
    ///          complex adjustment logic to the more detailed `adjust_overage` worker function.
    /// @param[in,out] fijk The FaceIJK with orientation to adjust. Its members (face, rotation, coordinates)
    ///                     will be modified in-place upon a successful adjustment.
    /// @param[in] res The resolution of the grid system.
    /// @param[in] digit The direction of movement that may have caused an overage.
    /// @return error_t::none on success, or an error propagated from the core adjustment logic.
    error_t adjust_overage(oriented_ijk& fijk, const resolution_t res, const direction_t digit) noexcept;

    /// @brief Determines the H3 base cell and resolution 0 orientation for a given FaceIJK.
    /// @details This is a key part of the reverse mapping from coordinates to an H3 index,
    /// corresponding to the core logic of `_faceIjkToH3` in the H3 C library.
    /// @param fijk The FaceIJK coordinates to find the base cell for.
    /// @param res The resolution of the provided `fijk`.
    /// @param[out] out_base_cell The determined base cell ID (0-121).
    /// @param[out] out_orientation The canonical orientation of the base cell at resolution 0.
    /// @return error_t::none on success, or an error code if no base cell could be found.
    error_t to_base_cell_and_orientation(const ijk& fijk, const resolution_t res, cell::base::id_t& out_base_cell,
                                         int& out_orientation) noexcept;

    /// @brief Simplified helper to determine the H3 base cell for a given FaceIJK.
    /// @param fijk The FaceIJK coordinates.
    /// @param res The resolution of the provided `fijk`.
    /// @return The base cell ID (0-121), or `base::invalid_index` on failure.
    cell::base::id_t to_base_cell(const ijk& fijk, const resolution_t res) noexcept;

    /// @brief Converts a FaceIJK representation at one resolution to its IJK coordinates at another.
    error_t to_ijk_at_resolution(const ijk& fijk_higher_res, const resolution_t res_higher, const resolution_t res_lower,
                                 coordinate::ijk& out_ijk_lower_res) noexcept;

    /// @brief Finds the FaceIJK of a neighboring cell, handling icosahedron face-crossing ("overage").
    /// @details This is a core internal function that orchestrates grid traversal. It corresponds to the
    ///          logic in H3's `_h3Neighbor` function. It first performs a simple IJK translation and
    ///          then checks if the new coordinates have exceeded the bounds of the current face's
    ///          coordinate system. If they have, it acts as a "traffic cop", calling the appropriate
    ///          low-level adjustment function (`adjust_hexagon_overage` or `adjust_overage`) based
    ///          on whether the origin cell was a hexagon or a pentagon.
    /// @param[in] start_fijk The FaceIJK of the starting cell.
    /// @param[in] res The H3 resolution of the grid.
    /// @param[in] dir The direction to move to find the neighbor.
    /// @param[out] out_neighbor_fijk The resulting FaceIJK of the neighbor cell.
    /// @return error_t::none on success, or an error code on failure.
    error_t find_neighbor_face_ijk(const oriented_ijk& start_fijk, const resolution_t res, const direction_t dir,
                                   oriented_ijk& out_neighbor_fijk) noexcept;

    std::int8_t find_vertex_on_face(const id_t face, const std::int8_t vertex_to_find) noexcept;
}
