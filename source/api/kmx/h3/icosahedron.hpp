/// @file h3/icosahedron.hpp
#pragma once
#ifndef PCH
    #include <kmx/gis/wgs84/coordinate.hpp>
    #include <kmx/h3/cell/pentagon.hpp>
    #include <kmx/h3/index.hpp>
    #include <kmx/math/vector.hpp>
#endif

namespace kmx::h3::icosahedron
{
    using face_no_t = std::uint8_t;

    enum class face_t : face_no_t
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
    constexpr face_no_t face_count = 20u;

    face_t face_of(const cell::base::id_t base_cell_id);

    bool is_cw_offset(const cell::base::id_t base_cell_id, const face_t face);

    /// @ref maxFaceCount
    inline std::uint8_t max_face_intersection_count(const index index) noexcept
    {
        return index.is_pentagon() ? 5u : 2u;
    }

    /// @ref getIcosahedronFaces
    void get_intersected_faces(const index index, std::span<face_no_t>& output) noexcept;

    using vector3 = math::vector3d;

    vector3 center_point(const face_t face) noexcept;

    using wgs_coord = gis::wgs84::coordinate;

    wgs_coord center_wgs(const face_t face) noexcept;
}
