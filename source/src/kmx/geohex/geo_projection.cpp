/// @file geohex/cell.cpp
#include "kmx/geohex/geo_projection.hpp"
#include <cmath>
#include <limits>

namespace kmx::geohex::cell
{
    // Static helper for the core math after scaling and rotation
    static error_t internal_final_uv_to_ijk(const math::vector2d& processed_uv, coordinate::ijk& out_ijk) noexcept
    {
        double j_axial = processed_uv.y / sqrt3_2;
        double i_axial = processed_uv.x - 0.5 * j_axial;
        double k_axial = -(i_axial + j_axial);

        coordinate::ijk::value i_int, j_int, k_int;
        cube_round<double, coordinate::ijk::value>(i_axial, j_axial, k_axial, i_int, j_int, k_int);

        out_ijk.i = i_int;
        out_ijk.j = j_int;
        out_ijk.k = k_int;
        out_ijk.normalize();
        return error_t::none;
    }

    // Public function that handles scaling and rotation before core conversion
    error_t convert_face_uv_to_ijk(const math::vector2d& raw_uv_on_face, resolution_t res, coordinate::ijk& out_ijk) noexcept
    {
        math::vector2d processed_uv = raw_uv_on_face;

        // Inverse scale by UNIT_VEC_SCALES for the resolution
        // TODO: Ensure icosahedron_data::get_unit_vec_scales() table is fully ported.
        double scale_factor = scaling_factor(res);
        if (std::fabs(scale_factor) < std::numeric_limits<double>::epsilon())
        {
            // Avoid division by zero / handle case where scale might be invalid (e.g. table not ported)
            // Allow res 0 scale to be small if that's how H3C table is.
            if (!(+res == 0 && std::fabs(scale_factor) < std::numeric_limits<double>::epsilon()))
            {
                out_ijk = {};
                return error_t::domain; // Or internal error if table is known to be bad
            }
            // If scale is effectively zero for res 0, treat as unscaled (uv coords are already small)
            // This needs to align with how H3C uses UNIT_VEC_SCALES.
            // If scale_factor is truly 0 for a valid res, behavior is undefined or error.
        }
        if (std::fabs(scale_factor) > std::numeric_limits<double>::epsilon())
        { // Only divide if scale is not effectively zero
            double inv_scale_factor = 1.0 / scale_factor;
            processed_uv.x *= inv_scale_factor;
            processed_uv.y *= inv_scale_factor;
        }

        // Inverse Class III rotation if resolution is Class III
        if (is_class_3(res))
        {
            constexpr auto THIRTY_DEGREES_RADS = std::numbers::pi_v<double> / 6.0;

            double angle_rad = -THIRTY_DEGREES_RADS; // Rotate CW to undo CCW effect on grid
            double cs = std::cos(angle_rad);
            double sn = std::sin(angle_rad);
            double old_x = processed_uv.x;
            processed_uv.x = old_x * cs - processed_uv.y * sn;
            processed_uv.y = old_x * sn + processed_uv.y * cs;
        }

        return internal_final_uv_to_ijk(processed_uv, out_ijk);
    }

    /// @ref _v3dToGeo (H3 C internal from algos.c)
    void from_v3d(const math::vector3d& v3, gis::wgs84::coordinate& out_coord) noexcept
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
    error_t face_ijk_to_v3d(const icosahedron::face::ijk& fijk_coords, resolution_t res, math::vector3d& out_v3) noexcept
    {
        // TODO implement
        return error_t::none;
    }

    /// @ref _v3dToFaceV2d
    /// @brief Projects a 3D point on the sphere to 2D UV coordinates on a specified face's plane.
    error_t project_v3d_to_face_uv(const math::vector3d& v3d, icosahedron::face::id_t face_num, math::vector2d& out_uv) noexcept
    {
        // TODO implement
        return error_t::none;
    }
}
