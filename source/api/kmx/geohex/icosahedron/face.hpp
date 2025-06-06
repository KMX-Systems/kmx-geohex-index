/// @file geohex/coordinate/icosahedron/face/ijk.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/cell/pentagon.hpp>
    #include <kmx/geohex/coordinate/ijk.hpp>
    #include <kmx/geohex/index.hpp>
    #include <kmx/gis/wgs84/coordinate.hpp>
    #include <kmx/math/vector.hpp>
    #include <optional>
#endif

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

    /// @brief Represents a coordinate on a specific icosahedron face with orientation.
    /// @note This is analogous to H3 C's internal `FaceOrientIJK` struct.
    struct oriented_ijk: ijk
    {
        int ccw_rotations_60 {}; ///< Counter-clockwise 60 deg rotations vs face canonical.

        constexpr bool operator==(const oriented_ijk&) const noexcept = default;
        constexpr bool operator!=(const oriented_ijk&) const noexcept = default;
    };

    /// @ref get_face_from_geo
    id_t from(const gis::wgs84::coordinate& coord) noexcept;

    /// @ref _h3ToFaceIjk (H3 C internal)
    /// @brief Converts an H3 cell index to its corresponding FaceIJK representation.
    /// @param index The H3 cell index.
    /// @param out Output: The FaceIJK representation.
    /// @return error_t::none on success, or an error code.
    error_t from(const index index, ijk& out) noexcept;

    /// @ref _faceIjkToGeo (H3 C internal, simplified here, full is _faceIjkToGeoEx)
    /// @brief Converts FaceIJK coordinates to geographic WGS84 coordinates (radians).
    /// @param fijk The FaceIJK to convert.
    /// @param res The resolution of the cell (needed for Class II/III projection adjustments).
    /// @param out_coord Output: Geographic coordinates in radians.
    /// @return error_t::none on success.
    error_t to_ijk(const gis::wgs84::coordinate& coord, const resolution_t res, ijk& out_fijk) noexcept;

    std::optional<ijk> get(const std::uint8_t pentagon_no, const direction_t direction) noexcept;

    /// @ref _adjustOverageClassII and _adjustOverageClassIII (H3 C internal)
    /// @brief Adjusts FaceIJK coordinates for pentagon distortion when crossing icosa face boundaries.
    /// @param current_face In/Out: The current icosahedron face. Updated if overage occurs.
    /// @param current_ccw_rotations_60 In/Out: Current system rotations. Updated if overage occurs.
    /// @param ijk_coords_to_adjust In/Out: The IJK coordinates to adjust. Translated/rotated if overage.
    /// @param current_res The current H3 resolution being processed.
    /// @param base_cell_id The ID of the pentagonal base cell.
    /// @param digit_moved The direction moved that might have caused overage.
    /// @return error_t::none on success, or an error if adjustment fails.
    error_t adjust_overage(id_t& current_face, int& current_ccw_rotations_60, coordinate::ijk& ijk_coords_to_adjust,
                           resolution_t current_res, cell::base::id_t base_cell_id, direction_t digit_moved) noexcept;
}
