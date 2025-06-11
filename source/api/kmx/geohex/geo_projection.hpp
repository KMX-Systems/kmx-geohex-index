/// @file geohex/geo_projection.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/icosahedron/face.hpp>
    #include <kmx/geohex/index.hpp>
    #include <kmx/math/vector.hpp>
#endif

namespace kmx::geohex::projection
{
    /// @ref _v3dToGeo (H3 C internal from algos.c)
    void from_v3d(const math::vector3d& v3, gis::wgs84::coordinate& out_coord) noexcept;

    /// @ref _geoToV3d (H3 C internal from algos.c)
    void to_v3d(const gis::wgs84::coordinate& geo_coord, math::vector3d& out_v3) noexcept;

    /// @ref _faceIjkToXYZ (H3 C internal, related to _faceIjkToGeoEx from faceijk.c)
    /// @brief Converts FaceIJK coordinates (cell center or vertex) to a 3D Cartesian vector.
    error_t face_ijk_to_v3d(const icosahedron::face::ijk& fijk_coords, resolution_t res, math::vector3d& out_v3) noexcept;

    /// @ref _v3dToFaceV2d
    /// @brief Projects a 3D point on the sphere to 2D UV coordinates on a specified face's plane.
    error_t project_v3d_to_face_uv(const math::vector3d& v3d, const icosahedron::face::id_t face_num, math::vector2d& out_uv) noexcept;

    /// @ref _hex2dToCoordIJK
    /// @brief Converts 2D UV coordinates on a face plane (after scaling/rotation) to IJK coordinates.
    /// @param raw_uv_on_face The UV coordinates directly from projection, before res-specific scaling/rotation.
    /// @param res The target resolution for the IJK.
    /// @param out_ijk Output IJK coordinates.
    error_t convert_face_uv_to_ijk(const math::vector2d& raw_uv_on_face, const resolution_t res, coordinate::ijk& out_ijk) noexcept;
}
