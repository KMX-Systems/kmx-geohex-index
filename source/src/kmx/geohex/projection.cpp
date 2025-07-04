/// @file src/kmx/geohex/projection.cpp
/// @ingroup Internal
#include "kmx/geohex/projection.hpp"
#include "kmx/geohex/utils.hpp"
#include <cmath>
#include <limits>

namespace kmx::geohex::projection
{
    /// @ref _v3dToGeo (H3 C internal from algos.c)
    void to_wgs(const math::vector3d& v3, gis::wgs84::coordinate& out_coord) noexcept
    {
        out_coord.latitude = std::asin(v3.z); // Latitude is arcsin(z)
        constexpr auto epsilon = std::numeric_limits<double>::epsilon();
        out_coord.longitude = (std::fabs(v3.x) < epsilon) && (std::fabs(v3.y) < epsilon) ? 0 : std::atan2(v3.y, v3.x);
    }

    /// @ref _geoToV3d (H3 C internal from algos.c)
    void to_v3d(const gis::wgs84::coordinate& geo_coord, math::vector3d& out_v3) noexcept
    {
        const double r = std::cos(geo_coord.latitude);
        out_v3.x = std::cos(geo_coord.longitude) * r;
        out_v3.y = std::sin(geo_coord.longitude) * r;
        out_v3.z = std::sin(geo_coord.latitude);
    }

    /// @ref _faceIjkToXYZ (H3 C internal, related to _faceIjkToGeoEx from faceijk.c)
    /// @brief Converts FaceIJK coordinates (cell center or vertex) to a 3D Cartesian vector.
    error_t to_v3d(const icosahedron::face::ijk& fijk_coords, resolution_t res, math::vector3d& out_v3) noexcept
    {
        // 1. Convert IJK to a 2D vector on the canonical hex grid.
        math::vector2d v2d = coordinate::to_vec2<double>(fijk_coords.ijk_coords);

        // 2. Scale by resolution-specific factor.
        v2d *= scaling_factor(res);

        // 3. Rotate for Class III (odd) resolutions.
        if (is_class_3(res))
        {
            constexpr double angle_rad = std::numbers::pi_v<double> / 6.0; // 30 degrees CCW
            const double cs = std::cos(angle_rad);
            const double sn = std::sin(angle_rad);
            const double old_x = v2d.x;
            v2d.x = old_x * cs - v2d.y * sn;
            v2d.y = old_x * sn + v2d.y * cs;
        }

        // 4. Project from the face's 2D gnomonic plane to the 3D sphere.
        // NOTE: This is a simplified projection. A production-quality implementation
        // requires porting the full `FaceOrient` data structures from H3, which define
        // the precise 3D orientation and projection parameters for each face.
        const auto face_center_v3 = icosahedron::face::center_point(fijk_coords.face);

        // Create an ad-hoc orthonormal basis for the tangent plane at the face center.
        // This is the most complex part to port correctly without the tables.
        // We'll use a standard approach to create a basis from a single vector.
        math::vector3d u_vec, v_vec;
        if (std::abs(face_center_v3.x) > std::abs(face_center_v3.y))
            u_vec = math::vector3d(-face_center_v3.z, 0, face_center_v3.x).normalized();
        else
            u_vec = math::vector3d(0, face_center_v3.z, -face_center_v3.y).normalized();

        v_vec = face_center_v3.cross(u_vec);
        out_v3 = (face_center_v3 + (u_vec * v2d.x) + (v_vec * v2d.y)).normalized();
        return error_t::none;
    }

    error_t to_ijk(const math::vector2d& raw_uv_on_face, const resolution_t res, coordinate::ijk& out_ijk) noexcept
    {
        math::vector2d processed_uv = raw_uv_on_face;

        // Inverse Class III rotation
        if (is_class_3(res))
        {
            constexpr auto angle_rad = -std::numbers::pi_v<double> / 6.0; // -30 degrees
            double cs = std::cos(angle_rad);
            double sn = std::sin(angle_rad);
            double old_x = processed_uv.x;
            processed_uv.x = old_x * cs - processed_uv.y * sn;
            processed_uv.y = old_x * sn + processed_uv.y * cs;
        }

        // Inverse scaling
        const double inv_scale_factor = 1.0 / scaling_factor(res);
        processed_uv.x *= inv_scale_factor;
        processed_uv.y *= inv_scale_factor;

        // Convert hex 2D coordinates to axial cube coordinates
        double j_axial = processed_uv.y / sqrt3_2;
        double i_axial = processed_uv.x - 0.5 * j_axial;

        // Round to integer cube coordinates
        coordinate::ijk::value i_int, j_int, k_int;
        cube_round(i_axial, j_axial, -(i_axial + j_axial), i_int, j_int, k_int);

        out_ijk = {i_int, j_int, k_int};
        return error_t::none;
    }

    /// @ref _v3dToFaceV2d
    /// @brief Projects a 3D point on the sphere to 2D UV coordinates on a specified face's plane.
    error_t to_face_uv(const math::vector3d& v3d, const icosahedron::face::id_t face_num, math::vector2d& out_uv) noexcept
    {
        // This is the inverse of to_v3d, performing a gnomonic projection.
        const auto face_center = icosahedron::face::center_point(face_num);

        // The point must be on the same hemisphere as the face center for gnomonic projection.
        if (v3d.dot(face_center) < 0.0)
            return error_t::failed; // Or a more specific error like DOMAIN

        // Use the same ad-hoc basis as in to_v3d for consistency.
        math::vector3d u_vec, v_vec;
        if (std::abs(face_center.x) > std::abs(face_center.y))
            u_vec = math::vector3d(-face_center.z, 0, face_center.x).normalized();
        else
            u_vec = math::vector3d(0, face_center.z, -face_center.y).normalized();

        v_vec = face_center.cross(u_vec);

        // Project v3d onto the tangent plane by scaling it to the plane.
        const math::vector3d p_prime = v3d / v3d.dot(face_center);

        // The vector from the plane's origin (face center) to the projected point.
        const math::vector3d p_prime_on_plane = p_prime - face_center;

        // Project onto the basis vectors to get the 2D coordinates.
        out_uv.x = p_prime_on_plane.dot(u_vec);
        out_uv.y = p_prime_on_plane.dot(v_vec);

        return error_t::none;
    }
}
