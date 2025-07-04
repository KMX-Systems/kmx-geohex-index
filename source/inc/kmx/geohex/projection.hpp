/// @file inc/kmx/geohex/geo_projection.hpp
/// @ingroup Internal
/// @brief Provides core geometric projection functions.
#pragma once
#ifndef PCH
    #include <kmx/geohex/icosahedron/face.hpp>
    #include <kmx/geohex/index.hpp>
    #include <kmx/math/vector.hpp>
#endif

namespace kmx::geohex::projection
{
    /// @brief Converts a 3D Cartesian vector on the unit sphere to WGS84 geographic coordinates.
    /// @ref _v3dToGeo (H3 C internal from algos.c)
    /// @param v3 The input 3D vector (must be a unit vector).
    /// @param[out] out_coord The structure to fill with the resulting latitude and longitude
    ///                       in radians.
    void to_wgs(const math::vector3d& v3, gis::wgs84::coordinate& out_coord) noexcept;

    /// @brief Converts WGS84 geographic coordinates to a 3D Cartesian vector on the unit sphere.
    /// @ref _geoToV3d (H3 C internal from algos.c)
    /// @param geo_coord The input geographic coordinate (latitude and longitude in radians).
    /// @param[out] out_v3 The structure to fill with the resulting X, Y, Z coordinates.
    void to_v3d(const gis::wgs84::coordinate& geo_coord, math::vector3d& out_v3) noexcept;

    /// @brief Converts FaceIJK coordinates to a 3D Cartesian vector on the unit sphere.
    /// @details This function takes a coordinate on a specific icosahedron face's planar
    ///          grid and projects it onto the 3D sphere. This is a critical step in
    ///          converting an H3 index or vertex back to a real-world geographic point.
    /// @ref _faceIjkToXYZ (H3 C internal, related to _faceIjkToGeoEx from faceijk.c)
    /// @param fijk_coords The FaceIJK representation of the point.
    /// @param res The H3 resolution of the grid system for the `fijk_coords`.
    /// @param[out] out_v3 The structure to fill with the resulting 3D vector.
    /// @return `error_t::none` on success.
    error_t to_v3d(const icosahedron::face::ijk& fijk_coords, resolution_t res, math::vector3d& out_v3) noexcept;

    /// @brief Projects a 3D point on the sphere to 2D UV coordinates on a specified face's plane.
    /// @details This function performs a gnomonic projection from the sphere onto the tangent
    ///          plane of a specific icosahedron face. This is a key step in converting a
    ///          geographic coordinate into H3's internal grid system.
    /// @ref _v3dToFaceV2d (H3 C internal)
    /// @param v3d The 3D unit vector to project.
    /// @param face_num The ID of the target icosahedron face.
    /// @param[out] out_uv The structure to fill with the resulting 2D UV coordinates on the
    ///                    face's plane.
    /// @return `error_t::none` on success. Returns an error if the point cannot be projected
    ///         onto the given face (e.g., it is on the wrong hemisphere).
    error_t to_face_uv(const math::vector3d& v3d, const icosahedron::face::id_t face_num, math::vector2d& out_uv) noexcept;

    /// @brief Converts 2D UV coordinates on a face plane to IJK grid coordinates.
    /// @details After a point has been projected onto a face's 2D plane, this function
    ///          applies the inverse resolution-specific scaling and rotation to transform
    ///          the planar UV coordinates into the discrete integer IJK cube coordinate system.
    /// @ref _hex2dToCoordIJK (H3 C internal)
    /// @param raw_uv_on_face The UV coordinates from the gnomonic projection, before any
    ///                       resolution-specific transformations.
    /// @param res The target H3 resolution for the IJK coordinates.
    /// @param[out] out_ijk The structure to fill with the resulting integer IJK coordinates.
    /// @return `error_t::none` on success.
    error_t to_ijk(const math::vector2d& raw_uv_on_face, const resolution_t res, coordinate::ijk& out_ijk) noexcept;
}
