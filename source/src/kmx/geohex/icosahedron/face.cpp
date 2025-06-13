/// @file src/kmx/geohex/icosahedron/face.cpp
/// @ingroup Internal
#include "kmx/geohex/icosahedron/face.hpp"
#include "kmx/geohex/cell/base.hpp"
#include "kmx/geohex/cell/boundary.hpp"
#include "kmx/geohex/projection.hpp"
#include <array>

namespace kmx::geohex::icosahedron::face
{
    static constexpr std::array<id_t, cell::base::count> face_data {
        id_t::f1,  // base cell 0
        id_t::f2,  // base cell 1
        id_t::f1,  // base cell 2
        id_t::f2,  // base cell 3
        id_t::f0,  // base cell 4
        id_t::f1,  // base cell 5
        id_t::f1,  // base cell 6
        id_t::f2,  // base cell 7
        id_t::f0,  // base cell 8
        id_t::f2,  // base cell 9
        id_t::f1,  // base cell 10
        id_t::f1,  // base cell 11
        id_t::f3,  // base cell 12
        id_t::f3,  // base cell 13
        id_t::f11, // base cell 14
        id_t::f4,  // base cell 15
        id_t::f0,  // base cell 16
        id_t::f6,  // base cell 17
        id_t::f0,  // base cell 18
        id_t::f2,  // base cell 19
        id_t::f7,  // base cell 20
        id_t::f2,  // base cell 21
        id_t::f0,  // base cell 22
        id_t::f6,  // base cell 23
        id_t::f10, // base cell 24
        id_t::f6,  // base cell 25
        id_t::f3,  // base cell 26
        id_t::f11, // base cell 27
        id_t::f4,  // base cell 28
        id_t::f3,  // base cell 29
        id_t::f0,  // base cell 30
        id_t::f4,  // base cell 31
        id_t::f5,  // base cell 32
        id_t::f0,  // base cell 33
        id_t::f7,  // base cell 34
        id_t::f11, // base cell 35
        id_t::f7,  // base cell 36
        id_t::f10, // base cell 37
        id_t::f12, // base cell 38
        id_t::f6,  // base cell 39
        id_t::f7,  // base cell 40
        id_t::f4,  // base cell 41
        id_t::f3,  // base cell 42
        id_t::f3,  // base cell 43
        id_t::f4,  // base cell 44
        id_t::f6,  // base cell 45
        id_t::f11, // base cell 46
        id_t::f8,  // base cell 47
        id_t::f5,  // base cell 48
        id_t::f14, // base cell 49
        id_t::f5,  // base cell 50
        id_t::f12, // base cell 51
        id_t::f10, // base cell 52
        id_t::f4,  // base cell 53
        id_t::f12, // base cell 54
        id_t::f7,  // base cell 55
        id_t::f11, // base cell 56
        id_t::f10, // base cell 57
        id_t::f13, // base cell 58
        id_t::f10, // base cell 59
        id_t::f11, // base cell 60
        id_t::f9,  // base cell 61
        id_t::f8,  // base cell 62
        id_t::f6,  // base cell 63
        id_t::f8,  // base cell 64
        id_t::f9,  // base cell 65
        id_t::f14, // base cell 66
        id_t::f5,  // base cell 67
        id_t::f16, // base cell 68
        id_t::f8,  // base cell 69
        id_t::f5,  // base cell 70
        id_t::f12, // base cell 71
        id_t::f7,  // base cell 72
        id_t::f12, // base cell 73
        id_t::f10, // base cell 74
        id_t::f9,  // base cell 75
        id_t::f13, // base cell 76
        id_t::f16, // base cell 77
        id_t::f15, // base cell 78
        id_t::f15, // base cell 79
        id_t::f16, // base cell 80
        id_t::f14, // base cell 81
        id_t::f13, // base cell 82
        id_t::f5,  // base cell 83
        id_t::f8,  // base cell 84
        id_t::f14, // base cell 85
        id_t::f9,  // base cell 86
        id_t::f14, // base cell 87
        id_t::f17, // base cell 88
        id_t::f12, // base cell 89
        id_t::f16, // base cell 90
        id_t::f17, // base cell 91
        id_t::f15, // base cell 92
        id_t::f16, // base cell 93
        id_t::f9,  // base cell 94
        id_t::f15, // base cell 95
        id_t::f13, // base cell 96
        id_t::f8,  // base cell 97
        id_t::f13, // base cell 98
        id_t::f17, // base cell 99
        id_t::f19, // base cell 100
        id_t::f14, // base cell 101
        id_t::f19, // base cell 102
        id_t::f17, // base cell 103
        id_t::f13, // base cell 104
        id_t::f17, // base cell 105
        id_t::f16, // base cell 106
        id_t::f9,  // base cell 107
        id_t::f15, // base cell 108
        id_t::f15, // base cell 109
        id_t::f18, // base cell 110
        id_t::f18, // base cell 111
        id_t::f19, // base cell 112
        id_t::f17, // base cell 113
        id_t::f19, // base cell 114
        id_t::f18, // base cell 115
        id_t::f18, // base cell 116
        id_t::f19, // base cell 117
        id_t::f19, // base cell 118
        id_t::f18, // base cell 119
        id_t::f19, // base cell 120
        id_t::f18, // base cell 121
    };

    constexpr id_t internal_of(const cell::base::id_t base_cell_id) noexcept
    {
        return face_data[base_cell_id];
    }

    id_t of(const cell::base::id_t base_cell_id) noexcept
    {
        return internal_of(base_cell_id);
    }

    bool is_cw_offset(const cell::base::id_t base_cell_id, const id_t face) noexcept
    {
        const auto offsets = cell::pentagon::clockwise_offsets(base_cell_id);
        const auto face_id = static_cast<cell::pentagon::clockwise_offset_t>(+face);
        return (offsets.first == face_id) || (offsets.second == face_id);
    }

    /// @brief Icosahedron face centers in x/y/z on the unit sphere.
    /// @ref faceCenterPoint
    static constexpr std::array<vector3, count> face_center_point {{
        {0.2199307791404606, 0.6583691780274996, 0.7198475378926182},    // face  0
        {-0.2139234834501421, 0.1478171829550703, 0.9656017935214205},   // face  1
        {0.1092625278784797, -0.4811951572873210, 0.8697775121287253},   // face  2
        {0.7428567301586791, -0.3593941678278028, 0.5648005936517033},   // face  3
        {0.8112534709140969, 0.3448953237639384, 0.4721387736413930},    // face  4
        {-0.1055498149613921, 0.9794457296411413, 0.1718874610009365},   // face  5
        {-0.8075407579970092, 0.1533552485898818, 0.5695261994882688},   // face  6
        {-0.2846148069787907, -0.8644080972654206, 0.4144792552473539},  // face  7
        {0.7405621473854482, -0.6673299564565524, -0.0789837646326737},  // face  8
        {0.8512303986474293, 0.4722343788582681, -0.2289137388687808},   // face  9
        {-0.7405621473854481, 0.6673299564565524, 0.0789837646326737},   // face 10
        {-0.8512303986474292, -0.4722343788582682, 0.2289137388687808},  // face 11
        {0.1055498149613919, -0.9794457296411413, -0.1718874610009365},  // face 12
        {0.8075407579970092, -0.1533552485898819, -0.5695261994882688},  // face 13
        {0.2846148069787908, 0.8644080972654204, -0.4144792552473539},   // face 14
        {-0.7428567301586791, 0.3593941678278027, -0.5648005936517033},  // face 15
        {-0.8112534709140971, -0.3448953237639382, -0.4721387736413930}, // face 16
        {-0.2199307791404607, -0.6583691780274996, -0.7198475378926182}, // face 17
        {0.2139234834501420, -0.1478171829550704, -0.9656017935214205},  // face 18
        {-0.1092625278784796, 0.4811951572873210, -0.8697775121287253}   // face 19
    }};

    vector3 center_point(const id_t face) noexcept
    {
        return face_center_point[+face];
    }

    /// @ref faceCenterGeo
    static constexpr std::array<std::array<double, 2u>, count> face_center_wgs {{
        {0.803582649718989942, 1.248397419617396099},   // face  0
        {1.307747883455638156, 2.536945009877921159},   // face  1
        {1.054751253523952054, -1.347517358900396623},  // face  2
        {0.600191595538186799, -0.450603909469755746},  // face  3
        {0.491715428198773866, 0.401988202911306943},   // face  4
        {0.172745327415618701, 1.678146885280433686},   // face  5
        {0.605929321571350690, 2.953923329812411617},   // face  6
        {0.427370518328979641, -1.888876200336285401},  // face  7
        {-0.079066118549212831, -0.733429513380867741}, // face  8
        {-0.230961644455383637, 0.506495587332349035},  // face  9
        {0.079066118549212831, 2.408163140208925497},   // face 10
        {0.230961644455383637, -2.635097066257444203},  // face 11
        {-0.172745327415618701, -1.463445768309359553}, // face 12
        {-0.605929321571350690, -0.187669323777381622}, // face 13
        {-0.427370518328979641, 1.252716453253507838},  // face 14
        {-0.600191595538186799, 2.690988744120037492},  // face 15
        {-0.491715428198773866, -2.739604450678486295}, // face 16
        {-0.803582649718989942, -1.893195233972397139}, // face 17
        {-1.307747883455638156, -0.604647643711872080}, // face 18
        {-1.054751253523952054, 1.794075294689396615}   // face 19
    }};

    gis::wgs84::coordinate center_wgs(const id_t face) noexcept
    {
        const auto& result = face_center_wgs[+face];
        return {result[0u], result[1u]};
    }

    /// @ref getIcosahedronFaces
    /// @brief Determines the set of icosahedron faces a given H3 cell intersects.
    error_t get_intersected(const index index, std::span<no_t>& output) noexcept
    {
        if (!index.is_valid())
        {
            output = {};
            return error_t::cell_invalid;
        }

        // Use a bitset for highly efficient, O(1) tracking of unique faces.
        std::bitset<count> faces_found;

        // 1. Get the cell's boundary vertices.
        // We create a span of the full buffer to pass to the function.
        std::array<gis::wgs84::coordinate, cell::boundary::max_vertices> boundary_data;
        std::span<gis::wgs84::coordinate> boundary_span = boundary_data;

        // We assume cell::boundary::get will resize `boundary_span` to the actual count.
        const error_t boundary_err = cell::boundary::get(index, boundary_span);
        if (boundary_err != error_t::none)
        {
            output = {};
            return boundary_err;
        }

        // 2. Collect faces from all relevant points (vertices + center).
        for (const auto& vertex_coord: boundary_span)
            faces_found.set(+from_wgs(vertex_coord));

        // Also check the cell center, which is essential for cells entirely within one face.
        gis::wgs84::coordinate cell_center;
        // Assuming the index can provide its center coordinate. This is a common requirement.
        // A free function to_wgs(index, out) is also plausible.
        const error_t center_err = index.to_wgs(cell_center);
        if (center_err != error_t::none)
        {
            output = {};
            return center_err;
        }

        faces_found.set(+from_wgs(cell_center));

        // 3. Write the unique faces found into the output buffer.
        std::size_t faces_written {};
        const std::size_t output_capacity = output.size();

        // Iterate through all possible face IDs (0-19).
        for (no_t i {}; i < count; ++i)
        {
            if (faces_found.test(i))
            {
                if (faces_written < output_capacity)
                    output[faces_written++] = i;
                else
                    // The provided buffer is full. Stop writing but do not return an error,
                    // as the operation was successful. The caller receives a truncated result.
                    break;
            }
        }

        // 4. Resize the caller's span to reflect the actual number of faces written.
        // This is the idiomatic way to return the count with this signature.
        output = output.subspan(0u, faces_written);

        return error_t::none;
    }

    id_t from_wgs(const gis::wgs84::coordinate& coord) noexcept
    {
        math::vector3d v3d;
        projection::to_v3d(coord, v3d);

        id_t best_face = id_t::f0;
        double max_dot = -2.0; // Dot products are in [-1, 1]

        for (no_t i {}; i < count; ++i)
        {
            const auto face_id = static_cast<id_t>(i);
            const auto face_center = center_point(face_id);
            const double dot = v3d.dot(face_center);
            if (dot > max_dot)
            {
                max_dot = dot;
                best_face = face_id;
            }
        }

        return best_face;
    }

    static constexpr std::uint8_t unique_ijk_instances = 8u;

    static constexpr std::array<pseudo_ijk, unique_ijk_instances> unique_pseudo_ijk_array {
        pseudo_ijk {0, 0, 0}, // Index 0
        pseudo_ijk {0, 0, 1}, // Index 1
        pseudo_ijk {0, 1, 0}, // Index 2
        pseudo_ijk {0, 1, 1}, // Index 3
        pseudo_ijk {1, 0, 0}, // Index 4
        pseudo_ijk {1, 0, 1}, // Index 5
        pseudo_ijk {1, 1, 0}, // Index 6
        pseudo_ijk {2, 0, 0}, // Index 7
    };

    struct home_fijk_data
    {
        std::uint8_t index; // index in unique_pseudo_ijk_array
        id_t face;
    };

    static constexpr std::array<home_fijk_data, cell::base::count> home_fijk_array {{
        {4u, id_t::f1},  // base cell 0:   {1, 0, 0}
        {6u, id_t::f2},  // base cell 1:   {1, 1, 0}
        {0u, id_t::f1},  // base cell 2:   {0, 0, 0}
        {4u, id_t::f2},  // base cell 3:   {1, 0, 0}
        {7u, id_t::f0},  // base cell 4:   {2, 0, 0}
        {6u, id_t::f1},  // base cell 5:   {1, 1, 0}
        {1u, id_t::f1},  // base cell 6:   {0, 0, 1}
        {0u, id_t::f2},  // base cell 7:   {0, 0, 0}
        {4u, id_t::f0},  // base cell 8:   {1, 0, 0}
        {2u, id_t::f2},  // base cell 9:   {0, 1, 0}
        {2u, id_t::f1},  // base cell 10:  {0, 1, 0}
        {3u, id_t::f1},  // base cell 11:  {0, 1, 1}
        {4u, id_t::f3},  // base cell 12:  {1, 0, 0}
        {6u, id_t::f3},  // base cell 13:  {1, 1, 0}
        {7u, id_t::f11}, // base cell 14:  {2, 0, 0}
        {4u, id_t::f4},  // base cell 15:  {1, 0, 0}
        {0u, id_t::f0},  // base cell 16:  {0, 0, 0}
        {2u, id_t::f6},  // base cell 17:  {0, 1, 0}
        {1u, id_t::f0},  // base cell 18:  {0, 0, 1}
        {3u, id_t::f2},  // base cell 19:  {0, 1, 1}
        {1u, id_t::f7},  // base cell 20:  {0, 0, 1}
        {1u, id_t::f2},  // base cell 21:  {0, 0, 1}
        {6u, id_t::f0},  // base cell 22:  {1, 1, 0}
        {1u, id_t::f6},  // base cell 23:  {0, 0, 1}
        {7u, id_t::f10}, // base cell 24:  {2, 0, 0}
        {0u, id_t::f6},  // base cell 25:  {0, 0, 0}
        {0u, id_t::f3},  // base cell 26:  {0, 0, 0}
        {4u, id_t::f11}, // base cell 27:  {1, 0, 0}
        {6u, id_t::f4},  // base cell 28:  {1, 1, 0}
        {2u, id_t::f3},  // base cell 29:  {0, 1, 0}
        {3u, id_t::f0},  // base cell 30:  {0, 1, 1}
        {0u, id_t::f4},  // base cell 31:  {0, 0, 0}
        {2u, id_t::f5},  // base cell 32:  {0, 1, 0}
        {2u, id_t::f0},  // base cell 33:  {0, 1, 0}
        {2u, id_t::f7},  // base cell 34:  {0, 1, 0}
        {6u, id_t::f11}, // base cell 35:  {1, 1, 0}
        {0u, id_t::f7},  // base cell 36:  {0, 0, 0}
        {4u, id_t::f10}, // base cell 37:  {1, 0, 0}
        {7u, id_t::f12}, // base cell 38:  {2, 0, 0}
        {5u, id_t::f6},  // base cell 39:  {1, 0, 1}
        {5u, id_t::f7},  // base cell 40:  {1, 0, 1}
        {1u, id_t::f4},  // base cell 41:  {0, 0, 1}
        {1u, id_t::f3},  // base cell 42:  {0, 0, 1}
        {3u, id_t::f3},  // base cell 43:  {0, 1, 1}
        {2u, id_t::f4},  // base cell 44:  {0, 1, 0}
        {4u, id_t::f6},  // base cell 45:  {1, 0, 0}
        {0u, id_t::f11}, // base cell 46:  {0, 0, 0}
        {1u, id_t::f8},  // base cell 47:  {0, 0, 1}
        {1u, id_t::f5},  // base cell 48:  {0, 0, 1}
        {7u, id_t::f14}, // base cell 49:  {2, 0, 0}
        {0u, id_t::f5},  // base cell 50:  {0, 0, 0}
        {4u, id_t::f12}, // base cell 51:  {1, 0, 0}
        {6u, id_t::f10}, // base cell 52:  {1, 1, 0}
        {3u, id_t::f4},  // base cell 53:  {0, 1, 1}
        {6u, id_t::f12}, // base cell 54:  {1, 1, 0}
        {4u, id_t::f7},  // base cell 55:  {1, 0, 0}
        {2u, id_t::f11}, // base cell 56:  {0, 1, 0}
        {0u, id_t::f10}, // base cell 57:  {0, 0, 0}
        {7u, id_t::f13}, // base cell 58:  {2, 0, 0}
        {1u, id_t::f10}, // base cell 59:  {0, 0, 1}
        {1u, id_t::f11}, // base cell 60:  {0, 0, 1}
        {2u, id_t::f9},  // base cell 61:  {0, 1, 0}
        {2u, id_t::f8},  // base cell 62:  {0, 1, 0}
        {7u, id_t::f6},  // base cell 63:  {2, 0, 0}
        {0u, id_t::f8},  // base cell 64:  {0, 0, 0}
        {1u, id_t::f9},  // base cell 65:  {0, 0, 1}
        {4u, id_t::f14}, // base cell 66:  {1, 0, 0}
        {5u, id_t::f5},  // base cell 67:  {1, 0, 1}
        {3u, id_t::f16}, // base cell 68:  {0, 1, 1}
        {5u, id_t::f8},  // base cell 69:  {1, 0, 1}
        {4u, id_t::f5},  // base cell 70:  {1, 0, 0}
        {0u, id_t::f12}, // base cell 71:  {0, 0, 0}
        {7u, id_t::f7},  // base cell 72:  {2, 0, 0}
        {2u, id_t::f12}, // base cell 73:  {0, 1, 0}
        {2u, id_t::f10}, // base cell 74:  {0, 1, 0}
        {0u, id_t::f9},  // base cell 75:  {0, 0, 0}
        {4u, id_t::f13}, // base cell 76:  {1, 0, 0}
        {1u, id_t::f16}, // base cell 77:  {0, 0, 1}
        {3u, id_t::f15}, // base cell 78:  {0, 1, 1}
        {2u, id_t::f15}, // base cell 79:  {0, 1, 0}
        {2u, id_t::f16}, // base cell 80:  {0, 1, 0}
        {6u, id_t::f14}, // base cell 81:  {1, 1, 0}
        {6u, id_t::f13}, // base cell 82:  {1, 1, 0}
        {7u, id_t::f5},  // base cell 83:  {2, 0, 0}
        {4u, id_t::f8},  // base cell 84:  {1, 0, 0}
        {0u, id_t::f14}, // base cell 85:  {0, 0, 0}
        {5u, id_t::f9},  // base cell 86:  {1, 0, 1}
        {1u, id_t::f14}, // base cell 87:  {0, 0, 1}
        {1u, id_t::f17}, // base cell 88:  {0, 0, 1}
        {1u, id_t::f12}, // base cell 89:  {0, 0, 1}
        {0u, id_t::f16}, // base cell 90:  {0, 0, 0}
        {3u, id_t::f17}, // base cell 91:  {0, 1, 1}
        {1u, id_t::f15}, // base cell 92:  {0, 0, 1}
        {5u, id_t::f16}, // base cell 93:  {1, 0, 1}
        {4u, id_t::f9},  // base cell 94:  {1, 0, 0}
        {0u, id_t::f15}, // base cell 95:  {0, 0, 0}
        {0u, id_t::f13}, // base cell 96:  {0, 0, 0}
        {7u, id_t::f8},  // base cell 97:  {2, 0, 0}
        {2u, id_t::f13}, // base cell 98:  {0, 1, 0}
        {5u, id_t::f17}, // base cell 99:  {1, 0, 1}
        {2u, id_t::f19}, // base cell 100: {0, 1, 0}
        {2u, id_t::f14}, // base cell 101: {0, 1, 0}
        {3u, id_t::f19}, // base cell 102: {0, 1, 1}
        {2u, id_t::f17}, // base cell 103: {0, 1, 0}
        {1u, id_t::f13}, // base cell 104: {0, 0, 1}
        {0u, id_t::f17}, // base cell 105: {0, 0, 0}
        {4u, id_t::f16}, // base cell 106: {1, 0, 0}
        {7u, id_t::f9},  // base cell 107: {2, 0, 0}
        {5u, id_t::f15}, // base cell 108: {1, 0, 1}
        {4u, id_t::f15}, // base cell 109: {1, 0, 0}
        {3u, id_t::f18}, // base cell 110: {0, 1, 1}
        {1u, id_t::f18}, // base cell 111: {0, 0, 1}
        {1u, id_t::f19}, // base cell 112: {0, 0, 1}
        {4u, id_t::f17}, // base cell 113: {1, 0, 0}
        {0u, id_t::f19}, // base cell 114: {0, 0, 0}
        {2u, id_t::f18}, // base cell 115: {0, 1, 0}
        {5u, id_t::f18}, // base cell 116: {1, 0, 1}
        {7u, id_t::f19}, // base cell 117: {2, 0, 0}
        {4u, id_t::f19}, // base cell 118: {1, 0, 0}
        {0u, id_t::f18}, // base cell 119: {0, 0, 0}
        {5u, id_t::f19}, // base cell 120: {1, 0, 1}
        {4u, id_t::f18}, // base cell 121: {1, 0, 0}
    }};

    ijk home(const cell::base::id_t base_id) noexcept
    {
        const auto& item = home_fijk_array[+base_id];
        return {coordinate::ijk {unique_pseudo_ijk_array[item.index]}, item.face};
    }

    /// @ref _h3ToFaceIjk (H3 C internal)
    /// @brief Converts an H3 cell index to its corresponding FaceIJK representation.
    /// @param index The H3 cell index.
    /// @param out Output: The FaceIJK representation.
    /// @return error_t::none on success, or an error code.
    error_t from(const index index, ijk& out) noexcept
    {
        if (!index.is_valid())
            return error_t::cell_invalid;

        const auto base_cell = index.base_cell();
        const auto res = index.resolution();

        // 1. Get the base cell's canonical orientation using direct lookups.
        // This is far more efficient than a hash map.
        oriented_ijk fijk_oriented;
        fijk_oriented.face = of(base_cell);                                                  // from the existing `face_data` array
        fijk_oriented.ijk_coords = {};                                                       // by definition
        fijk_oriented.ccw_rotations_60 = cell::pentagon::clockwise_offsets(base_cell).first; // from the new array

        // 2. From res 1 up to the cell's resolution, apply child digits to descend the tree.
        for (resolution_t r = resolution_t::r1; +r <= +res; r = static_cast<resolution_t>(+r + 1u))
        {
            // Move to the center of the finer resolution grid.
            auto& ijk_coords = fijk_oriented.ijk_coords;
            if (is_class_3(r))
                ijk_coords.down_ap7r();
            else
                ijk_coords.down_ap7();

            const auto digit = static_cast<direction_t>(index.digit(+r - 1));

            // Rotate the child digit to match the base cell's orientation
            auto rotated_digit_ijk = coordinate::to_ijk(digit);
            for (int i = 0; i < fijk_oriented.ccw_rotations_60; ++i)
                rotated_digit_ijk.rotate_60ccw();

            // Apply child digit vector
            fijk_oriented.ijk_coords += rotated_digit_ijk;

            // For pentagons, we must also apply local rotations based on the direction taken.
            if (cell::pentagon::check(base_cell))
            {
                const auto& local_rots = cell::base::rotations_60ccw(base_cell);
                for (int i = 0; i < local_rots[+digit]; ++i)
                    fijk_oriented.ijk_coords.rotate_60ccw();

                // TODO: A full implementation would call `adjust_overage` here if crossing face boundaries.
            }

            ijk_coords.normalize();
        }

        out = fijk_oriented; // Copy face and ijk_coords
        return error_t::none;
    }

    static constexpr std::array<math::vector3d, 12u> icosahedron_vertices {{
        {0.85065080835204, 0.00000000000000, 0.52573111211913},
        {0.85065080835204, 0.00000000000000, -0.52573111211913},
        {0.52573111211913, -0.85065080835204, 0.00000000000000},
        {0.00000000000000, -0.52573111211913, 0.85065080835204},
        {-0.52573111211913, -0.85065080835204, 0.00000000000000},
        {-0.85065080835204, 0.00000000000000, 0.52573111211913},
        {-0.52573111211913, 0.85065080835204, 0.00000000000000},
        {0.00000000000000, 0.52573111211913, 0.85065080835204},
        {0.52573111211913, 0.85065080835204, 0.00000000000000},
        {-0.85065080835204, 0.00000000000000, -0.52573111211913},
        {0.00000000000000, -0.52573111211913, -0.85065080835204},
        {0.00000000000000, 0.52573111211913, -0.85065080835204},
    }};

    /// @brief Defines which 3 of the 12 vertices form each of the 20 faces.
    /// The vertex indices are ordered counter-clockwise.
    using face_vertices_t = std::array<std::int8_t, 3u>;
    static constexpr std::array<face_vertices_t, count> face_to_vertices {
        {{{0, 8, 7}},  {{0, 3, 2}},  {{0, 2, 1}},  {{0, 7, 5}},  {{0, 5, 3}},   {{8, 0, 1}},  {{2, 3, 4}},
         {{3, 5, 4}},  {{5, 7, 6}},  {{7, 8, 6}},  {{1, 2, 10}}, {{4, 2, 10}},  {{9, 5, 6}},  {{4, 5, 9}},
         {{6, 8, 11}}, {{8, 1, 11}}, {{9, 6, 11}}, {{10, 2, 4}}, {{1, 10, 11}}, {{9, 11, 10}}}};

    /// @brief For each face, lists its 3 neighboring faces in CCW order.
    /// A value of -1 indicates no neighbor in that direction (should not happen).
    using face_neighbors_t = std::array<std::int8_t, 3u>;
    static constexpr std::array<face_neighbors_t, count> face_neighbors {
        {{{15, 9, 1}},  {{0, 2, 4}},  {{1, 10, 5}},   {{0, 4, 8}},    {{1, 3, 7}},    {{0, 10, 2}},  {{9, 8, 14}},
         {{4, 13, 11}}, {{3, 12, 6}}, {{0, 14, 7}},   {{1, 5, 11}},   {{7, 17, 10}},  {{8, 16, 13}}, {{7, 12, 17}},
         {{9, 16, 15}}, {{0, 18, 9}}, {{12, 14, 18}}, {{11, 13, 19}}, {{15, 16, 19}}, {{17, 18, 16}}}};

    /// @brief Gets the neighboring face by traversing across a shared vertex.
    /// @param face The starting face.
    /// @param vertex The index of the vertex on the starting face (0, 1, or 2).
    /// @return The ID of the neighboring face.
    static id_t get_neighbor_face(const id_t face, const std::int8_t vertex) noexcept
    {
        return static_cast<id_t>(face_neighbors[+face][vertex]);
    }

    /// @ref _geoToFaceIjk (H3 C internal)
    /// @brief Converts geographic WGS84 coordinates (radians) to FaceIJK representation.
    error_t from_wgs(const gis::wgs84::coordinate& coord, const resolution_t res, ijk& out_fijk) noexcept
    {
        math::vector3d v3d;
        projection::to_v3d(coord, v3d);
        id_t center_face = from_wgs(coord);

        math::vector2d uv;
        if (projection::v3d_to_face_uv(v3d, center_face, uv) != error_t::none)
            return error_t::failed;

        coordinate::ijk center_ijk_coords;
        if (projection::face_uv_to_ijk(uv, res, center_ijk_coords) != error_t::none)
            return error_t::failed;

        const auto center_v2d = coordinate::to_vec2<double>(center_ijk_coords);
        double min_dist_sq = hex2d_distance_sq(uv, center_v2d);

        out_fijk.face = center_face;
        out_fijk.ijk_coords = center_ijk_coords;

        std::array<id_t, count> face_queue;
        int queue_pos = 0;
        int queue_len = 1;
        face_queue[0] = center_face;
        std::array<bool, count> checked_faces {};
        checked_faces[+center_face] = true;

        while (queue_pos < queue_len)
        {
            const id_t current_face = face_queue[queue_pos++];
            for (int i = 0; i < 3; ++i)
            {
                id_t neighbor_face = get_neighbor_face(current_face, i);
                if (checked_faces[+neighbor_face])
                    continue;
                checked_faces[+neighbor_face] = true;

                math::vector2d neighbor_uv;
                if (projection::v3d_to_face_uv(v3d, neighbor_face, neighbor_uv) != error_t::none)
                    continue;

                coordinate::ijk neighbor_ijk_coords;
                projection::face_uv_to_ijk(neighbor_uv, res, neighbor_ijk_coords);

                const auto neighbor_v2d = coordinate::to_vec2<double>(neighbor_ijk_coords);
                const double dist_sq = hex2d_distance_sq(neighbor_uv, neighbor_v2d);

                if (dist_sq < min_dist_sq)
                {
                    min_dist_sq = dist_sq;
                    out_fijk.face = neighbor_face;
                    out_fijk.ijk_coords = neighbor_ijk_coords;
                    queue_pos = 0;
                    queue_len = 1;
                    face_queue[0] = out_fijk.face;
                    checked_faces.fill(false);
                    checked_faces[+out_fijk.face] = true;
                    break;
                }
                if (queue_len < count)
                {
                    face_queue[queue_len++] = neighbor_face;
                }
            }
        }

        const auto final_base_cell = to_base_cell(out_fijk, res);
        if (cell::pentagon::check(final_base_cell) && !is_class_3(res))
        {
            coordinate::ijk temp_ijk = out_fijk.ijk_coords;
            temp_ijk.normalize();
            if (temp_ijk.leading_digit(res) == direction_t::ik_axes)
            {
                out_fijk.ijk_coords.rotate_60cw();
            }
        }
        return error_t::none;
    }

    std::optional<ijk> get(const std::uint8_t pentagon_no, const direction_t direction) noexcept
    {
        using ijk_tuple = std::tuple<std::int8_t, std::int8_t, std::int8_t>;
        using face_ijk_tuple = std::tuple<ijk_tuple, id_t>;
        using face_ijk_tuple_array_t = std::array<face_ijk_tuple, direction_count - 2u>;
        using table_t = std::array<face_ijk_tuple_array_t, cell::pentagon::count>;

        static constexpr table_t data //
                                      // Pentagon 0 (BC 4)
            {{{{
                  {{2, 0, 0}, id_t::f4}, // J
                  {{0, 2, 0}, id_t::f8}, // JK
                  {{0, 0, 2}, id_t::f0}, // I
                  {{0, 0, 2}, id_t::f3}, // IK
                  {{2, 0, 0}, id_t::f1}  // IJ
              }},
              // Pentagon 1 (BC 14)
              {{
                  {{0, 0, 2}, id_t::f1},  // J
                  {{2, 0, 0}, id_t::f6},  // JK
                  {{0, 0, 2}, id_t::f11}, // I
                  {{2, 0, 0}, id_t::f7},  // IK
                  {{0, 2, 0}, id_t::f2}   // IJ
              }},
              // Pentagon 2 (BC 24)
              {{
                  {{2, 0, 0}, id_t::f5},  // J
                  {{0, 2, 0}, id_t::f0},  // JK
                  {{0, 0, 2}, id_t::f10}, // I
                  {{0, 2, 0}, id_t::f1},  // IK
                  {{0, 0, 2}, id_t::f6}   // IJ
              }},
              // Pentagon 3 (BC 38)
              {{
                  {{0, 0, 2}, id_t::f2},  // J
                  {{0, 0, 2}, id_t::f7},  // JK
                  {{0, 0, 2}, id_t::f12}, // I
                  {{2, 0, 0}, id_t::f3},  // IK
                  {{0, 2, 0}, id_t::f8}   // IJ
              }},
              // Pentagon 4 (BC 49)
              {{
                  {{2, 0, 0}, id_t::f9},  // J
                  {{0, 2, 0}, id_t::f4},  // JK
                  {{0, 0, 2}, id_t::f14}, // I
                  {{2, 0, 0}, id_t::f0},  // IK
                  {{0, 0, 2}, id_t::f5}   // IJ
              }},
              // Pentagon 5 (BC 58)
              {{
                  {{0, 2, 0}, id_t::f3},  // J
                  {{0, 0, 2}, id_t::f4},  // JK
                  {{0, 0, 2}, id_t::f13}, // I
                  {{2, 0, 0}, id_t::f8},  // IK
                  {{0, 2, 0}, id_t::f9}   // IJ
              }},
              // Pentagon 6 (BC 63)
              {{
                  {{0, 2, 0}, id_t::f16}, // J
                  {{2, 0, 0}, id_t::f11}, // JK
                  {{0, 0, 2}, id_t::f6},  // I
                  {{2, 0, 0}, id_t::f1},  // IK
                  {{0, 0, 2}, id_t::f15}  // IJ
              }},
              // Pentagon 7 (BC 72)
              {{
                  {{0, 2, 0}, id_t::f17}, // J
                  {{2, 0, 0}, id_t::f12}, // JK
                  {{0, 0, 2}, id_t::f7},  // I
                  {{2, 0, 0}, id_t::f2},  // IK
                  {{0, 0, 2}, id_t::f16}  // IJ
              }},
              // Pentagon 8 (BC 83)
              {{
                  {{0, 2, 0}, id_t::f19}, // J
                  {{2, 0, 0}, id_t::f14}, // JK
                  {{0, 0, 2}, id_t::f5},  // I
                  {{0, 2, 0}, id_t::f0},  // IK
                  {{0, 0, 2}, id_t::f10}  // IJ
              }},
              // Pentagon 9 (BC 97)
              {{
                  {{0, 2, 0}, id_t::f18}, // J
                  {{2, 0, 0}, id_t::f13}, // JK
                  {{0, 0, 2}, id_t::f8},  // I
                  {{0, 2, 0}, id_t::f3},  // IK
                  {{0, 0, 2}, id_t::f17}  // IJ
              }},
              // Pentagon 10 (BC 107)
              {{
                  {{2, 0, 0}, id_t::f19}, // J
                  {{2, 0, 0}, id_t::f18}, // JK
                  {{0, 0, 2}, id_t::f9},  // I
                  {{2, 0, 0}, id_t::f4},  // IK
                  {{0, 0, 2}, id_t::f14}  // IJ
              }},
              // Pentagon 11 (BC 117)
              {{
                  {{2, 0, 0}, id_t::f10}, // J
                  {{2, 0, 0}, id_t::f5},  // JK
                  {{0, 0, 2}, id_t::f19}, // I
                  {{0, 2, 0}, id_t::f9},  // IK
                  {{2, 0, 0}, id_t::f15}  // IJ
              }}}};

        if (pentagon_no < count)
            switch (direction)
            {
                case direction_t::j_axes:
                case direction_t::jk_axes:
                case direction_t::i_axes:
                case direction_t::ik_axes:
                case direction_t::ij_axes:
                {
                    constexpr std::uint8_t direction_offset = 2u;
                    const auto& source = data[pentagon_no][+direction - direction_offset];
                    return ijk {std::get<0>(source), std::get<1>(source)};
                }
                default:
                    break;
            };

        return {};
    }

    using radians_array_t = std::array<double, count>;

    /// @brief Icosahedron face ijk axes as azimuth in radians from face center to vertex 0.
    /// @ref faceAxesAzRadsCII
    static constexpr radians_array_t azimuth_rads_c2_vertex0 {
        5.619958268523939882, // face 0
        5.760339081714187279, // face 1
        0.780213654393430055, // face 2
        0.430469363979999913, // face 3
        6.130269123335111400, // face 4
        2.692877706530642877, // face 5
        2.982963003477243874, // face 6
        3.532912002790141181, // face 7
        3.494305004259568154, // face 8
        3.003214169499538391, // face 9
        5.930472956509811562, // face 10
        0.138378484090254847, // face 11
        0.448714947059150361, // face 12
        0.158629650112549365, // face 13
        5.891865957979238535, // face 14
        2.711123289609793325, // face 15
        3.294508837434268316, // face 16
        3.804819692245439833, // face 17
        3.664438879055192436, // face 18
        2.361378999196363184  // face 19
    };

    /// @brief Icosahedron face ijk axes as azimuth in radians from face center to vertex 1.
    /// @ref faceAxesAzRadsCII
    static constexpr radians_array_t azimuth_rads_c2_vertex1 {
        3.525563166130744542, // face 0
        3.665943979320991689, // face 1
        4.969003859179821079, // face 2
        4.619259568766391033, // face 3
        4.035874020941915804, // face 4
        0.598482604137447119, // face 5
        0.888567901084048369, // face 6
        1.438516900396945656, // face 7
        1.399909901866372864, // face 8
        0.908819067106342928, // face 9
        3.836077854116615875, // face 10
        4.327168688876645809, // face 11
        4.637505151845541521, // face 12
        4.347419854898940135, // face 13
        3.797470855586042958, // face 14
        0.616728187216597771, // face 15
        1.200113735041072948, // face 16
        1.710424589852244509, // face 17
        1.570043776661997111, // face 18
        0.266983896803167583  // face 19
    };

    /// @brief Icosahedron face ijk axes as azimuth in radians from face center to vertex 2.
    /// @ref faceAxesAzRadsCII
    static constexpr radians_array_t azimuth_rads_c2_vertex2 {
        1.431168063737548730, // face 0
        1.571548876927796127, // face 1
        2.874608756786625655, // face 2
        2.524864466373195467, // face 3
        1.941478918548720291, // face 4
        4.787272808923838195, // face 5
        5.077358105870439581, // face 6
        5.627307105183336758, // face 7
        5.588700106652763840, // face 8
        5.097609271892733906, // face 9
        1.741682751723420374, // face 10
        2.232773586483450311, // face 11
        2.543110049452346120, // face 12
        2.253024752505744869, // face 13
        1.703075753192847583, // face 14
        4.805518392002988683, // face 15
        5.388903939827463911, // face 16
        5.899214794638635174, // face 17
        5.758833981448388027, // face 18
        4.455774101589558636  // face 19
    };

    /// @brief A flat array of all base cell IDs, pre-sorted by their home face ID.
    static constexpr std::array<cell::base::id_t, cell::base::count> flat_data_sorted_by_face //
        {{                                                                                    //
          // Face 0 (7)
          4u, 8u, 16u, 18u, 22u, 30u, 33u,
          // Face 1 (6)
          0u, 2u, 5u, 6u, 10u, 11u,
          // Face 2 (6)
          1u, 3u, 7u, 9u, 19u, 21u,
          // Face 3 (6)
          12u, 13u, 26u, 29u, 42u, 43u,
          // Face 4 (6)
          15u, 28u, 31u, 41u, 44u, 53u,
          // Face 5 (6)
          32u, 48u, 50u, 67u, 70u, 83u,
          // Face 6 (6)
          17u, 23u, 25u, 39u, 45u, 63u,
          // Face 7 (6)
          20u, 34u, 36u, 40u, 55u, 72u,
          // Face 8 (6)
          47u, 62u, 64u, 69u, 84u, 97u,
          // Face 9 (6)
          61u, 65u, 75u, 86u, 94u, 107u,
          // Face 10 (6)
          24u, 37u, 52u, 57u, 59u, 74u,
          // Face 11 (6)
          14u, 27u, 35u, 46u, 56u, 60u,
          // Face 12 (6)
          38u, 51u, 54u, 71u, 73u, 89u,
          // Face 13 (6)
          58u, 76u, 82u, 96u, 98u, 104u,
          // Face 14 (6)
          49u, 66u, 81u, 85u, 87u, 101u,
          // Face 15 (6)
          78u, 79u, 92u, 95u, 108u, 109u,
          // Face 16 (6)
          68u, 77u, 80u, 90u, 93u, 106u,
          // Face 17 (6)
          88u, 91u, 99u, 103u, 105u, 113u,
          // Face 18 (6)
          110u, 111u, 115u, 116u, 119u, 121u,
          // Face 19 (7)
          100u, 102u, 112u, 114u, 117u, 118u, 120u}};

    /// @brief A `constexpr` array of spans, providing direct access to base cells for a given face.
    static constexpr std::array<std::span<const cell::base::id_t>, count> face_lookup_spans {{
        std::span(flat_data_sorted_by_face.data() + 0u, 7u),   // Face 0
        std::span(flat_data_sorted_by_face.data() + 7u, 6u),   // Face 1
        std::span(flat_data_sorted_by_face.data() + 13u, 6u),  // Face 2
        std::span(flat_data_sorted_by_face.data() + 19u, 6u),  // Face 3
        std::span(flat_data_sorted_by_face.data() + 25u, 6u),  // Face 4
        std::span(flat_data_sorted_by_face.data() + 31u, 6u),  // Face 5
        std::span(flat_data_sorted_by_face.data() + 37u, 6u),  // Face 6
        std::span(flat_data_sorted_by_face.data() + 43u, 6u),  // Face 7
        std::span(flat_data_sorted_by_face.data() + 49u, 6u),  // Face 8
        std::span(flat_data_sorted_by_face.data() + 55u, 6u),  // Face 9
        std::span(flat_data_sorted_by_face.data() + 61u, 6u),  // Face 10
        std::span(flat_data_sorted_by_face.data() + 67u, 6u),  // Face 11
        std::span(flat_data_sorted_by_face.data() + 73u, 6u),  // Face 12
        std::span(flat_data_sorted_by_face.data() + 79u, 6u),  // Face 13
        std::span(flat_data_sorted_by_face.data() + 85u, 6u),  // Face 14
        std::span(flat_data_sorted_by_face.data() + 91u, 6u),  // Face 15
        std::span(flat_data_sorted_by_face.data() + 97u, 6u),  // Face 16
        std::span(flat_data_sorted_by_face.data() + 103u, 6u), // Face 17
        std::span(flat_data_sorted_by_face.data() + 109u, 6u), // Face 18
        std::span(flat_data_sorted_by_face.data() + 115u, 7u)  // Face 19
    }};

    error_t from_index(const index index, ijk& out) noexcept
    {
        if (!index.is_valid())
            return error_t::cell_invalid;

        const auto base_cell = index.base_cell();
        const auto res = index.resolution();

        oriented_ijk fijk_oriented;
        fijk_oriented.face = of(base_cell);

        auto& ijk_coords = fijk_oriented.ijk_coords;
        ijk_coords = {};

        fijk_oriented.ccw_rotations_60 = cell::base::get_canonical_orientation(base_cell);

        for (resolution_t r = resolution_t::r1; +r <= +res; r = static_cast<resolution_t>(+r + 1u))
        {
            if (is_class_3(r))
                ijk_coords.down_ap7r();
            else
                ijk_coords.down_ap7();

            const auto digit = static_cast<direction_t>(index.digit(+r - 1u));
            auto rotated_digit_ijk = coordinate::to_ijk(digit);
            for (int i {}; i < fijk_oriented.ccw_rotations_60; ++i)
                rotated_digit_ijk.rotate_60ccw();

            ijk_coords += rotated_digit_ijk;
            if (cell::pentagon::check(base_cell))
            {
                const auto& local_rots = cell::base::rotations_60ccw(base_cell);
                for (int i {}; i < local_rots[+digit]; ++i)
                    fijk_oriented.ijk_coords.rotate_60ccw();
            }

            fijk_oriented.ijk_coords.normalize();
        }

        out = fijk_oriented;
        return error_t::none;
    }

    // This is the full inverse of `from_index`. It takes a FaceIJK and produces
    // a valid H3 index. This is the missing link needed for vertex creation.
    // In a real project, this would live in the `icosahedron::face` namespace.
    error_t to_index(const ijk& fijk, const resolution_t res, index& out_index) noexcept
    {
        // 1. First, determine the base cell for the given FaceIJK.
        cell::base::id_t base_cell;
        int orientation; // This will be the orientation of the path *from* the base cell.
        if (to_base_cell_and_orientation(fijk, res, base_cell, orientation) != error_t::none)
            return error_t::failed;

        // 2. Set the constant parts of the output index.
        out_index.set_resolution(res);
        out_index.set_base_cell(base_cell);
        out_index.set_mode(index_mode_t::cell); // Assuming cell mode for now.

        // 3. Now, determine the digits for each resolution from 1 to `res`.
        for (resolution_t r = resolution_t::r1; +r <= +res; r = static_cast<resolution_t>(+r + 1u))
        {
            // a. Find the IJK coordinates of our target `fijk` at the current resolution `r`.
            coordinate::ijk ijk_at_res_r;
            if (to_ijk_at_resolution(fijk, res, r, ijk_at_res_r) != error_t::none)
                return error_t::failed;

            // b. Find the IJK coordinates of the parent cell (at resolution r-1).
            coordinate::ijk parent_ijk;
            if (to_ijk_at_resolution(fijk, res, static_cast<resolution_t>(+r - 1u), parent_ijk) != error_t::none)
                return error_t::failed;

            // c. Scale the parent's IJK down to the grid of resolution `r`.
            parent_ijk = parent_ijk.down_ap7(is_class_3(r));

            // d. The difference between the cell and its scaled parent is the digit.
            const coordinate::ijk diff = ijk_at_res_r - parent_ijk;

            out_index.set_digit(+r - 1u, +diff.to_digit());
        }

        return error_t::none;
    }

    error_t to_wgs(const ijk& fijk, const resolution_t res, gis::wgs84::coordinate& out_coord) noexcept
    {
        math::vector3d v3d;
        if (projection::face_ijk_to_v3d(fijk, res, v3d) != error_t::none)
            return error_t::failed;

        projection::to_wgs(v3d, out_coord);
        return error_t::none;
    }

    error_t to_base_cell_and_orientation(const ijk& fijk, const resolution_t res, cell::base::id_t& out_base_cell,
                                         int& out_orientation) noexcept
    {
        oriented_ijk fijk_oriented;
        fijk_oriented.face = fijk.face;
        fijk_oriented.ijk_coords = fijk.ijk_coords;
        fijk_oriented.ccw_rotations_60 = {};

        auto& ijk_coords = fijk_oriented.ijk_coords;

        // Ascend the grid from the cell's resolution up to resolution 0.
        for (resolution_t r = res; +r > 0u; r = static_cast<resolution_t>(+r - 1u))
        {
            coordinate::ijk last_ijk = ijk_coords;
            if (is_class_3(r))
                ijk_coords.up_ap7r();
            else
                ijk_coords.up_ap7();

            coordinate::ijk diff = last_ijk - ijk_coords;
            diff.scale(is_class_3(r) ? 3 : 7);
            coordinate::ijk rotated_parent_ijk = last_ijk - diff;

            if (ijk_coords != rotated_parent_ijk)
            {
                coordinate::ijk tmp = ijk_coords;
                int rotations {};
                while (tmp != rotated_parent_ijk)
                {
                    tmp.rotate_60ccw();
                    rotations++;
                    if (rotations > 6)
                        return error_t::failed;
                }

                fijk_oriented.ccw_rotations_60 = (fijk_oriented.ccw_rotations_60 + rotations) % 6;
            }
        }

        // At res 0, look up the base cell using the compile-time generated table.
        // This now refers directly to `face_lookup_spans` for maximum clarity.
        const auto& candidates = face_lookup_spans[+fijk_oriented.face];
        for (const cell::base::id_t candidate_bc: candidates)
        {
            // A res 0 FaceIJK from a valid ascent must have ijk coords {0,0,0}.
            if (fijk_oriented.ijk_coords.is_origin())
            {
                out_base_cell = candidate_bc;
                int canonical_orientation = cell::base::get_canonical_orientation(out_base_cell);
                out_orientation = (canonical_orientation + fijk_oriented.ccw_rotations_60) % 6;
                return error_t::none;
            }
        }

        return error_t::cell_invalid;
    }

    cell::base::id_t to_base_cell(const ijk& fijk, const resolution_t res) noexcept
    {
        cell::base::id_t base_cell = cell::base::invalid_index;
        int orientation {};
        if (to_base_cell_and_orientation(fijk, res, base_cell, orientation) == error_t::none)
            return base_cell;

        return cell::base::invalid_index;
    }

    error_t to_ijk_at_resolution(const ijk& fijk_higher_res, const resolution_t res_higher, const resolution_t res_lower,
                                 coordinate::ijk& out_ijk_lower_res) noexcept
    {
        // To convert from a high-res IJK to a low-res IJK, we can't just scale.
        // The most robust way is to convert the high-res FaceIJK to a 3D vector,
        // then re-project that vector onto the same face and convert it back to IJK
        // at the desired *lower* resolution.

        // 1. Convert the high-resolution FaceIJK to a 3D vector.
        math::vector3d v3d;
        if (projection::face_ijk_to_v3d(fijk_higher_res, res_higher, v3d) != error_t::none)
            return error_t::failed;

        // 2. Project that 3D vector onto the same face's 2D gnomonic plane.
        math::vector2d uv;
        if (projection::v3d_to_face_uv(v3d, fijk_higher_res.face, uv) != error_t::none)
            return error_t::failed;

        // 3. Convert the 2D UV coordinates to an IJK grid at the desired *lower* resolution.
        return projection::face_uv_to_ijk(uv, res_lower, out_ijk_lower_res);
    }

    /// @brief Rotations to apply when moving from a pentagon to a neighboring face.
    /// @details This is a port of `PENTAGON_ROTATIONS` from H3's `pentagon.c`.
    static constexpr std::array<std::int8_t, cell::pentagon::count> pentagon_leading_digit_rotations = {1, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 2};

    /// @brief Standard face-to-face rotations for hexagon-to-hexagon overage.
    /// @details Port of `adjFaceRotations` from H3's `faceijk.c`.
    static constexpr std::array<std::array<int, 3u>, count> adj_face_rotations = {
        {{3, 0, 0}, {0, 0, 3}, {0, 3, 0}, {0, 0, 3}, {0, 3, 0}, {3, 0, 0}, {0, 0, 3}, {0, 3, 0}, {0, 0, 3}, {3, 0, 0},
         {3, 0, 0}, {0, 3, 0}, {3, 0, 0}, {0, 3, 0}, {0, 0, 3}, {0, 0, 3}, {3, 0, 0}, {0, 3, 0}, {3, 0, 0}, {0, 3, 0}}};

    /// @brief Standard face-to-face coordinate translations for hexagon-to-hexagon overage.
    /// @details Port of `adjFaceOffsets` from H3's `faceijk.c`.
    static constexpr std::array<std::array<pseudo_ijk, 3u>, count> adj_face_offsets {{
        {{{1, 1, 0}, {2, 0, 0}, {1, 0, 1}}}, // Face 0
        {{{0, 0, 1}, {0, 1, 1}, {1, 1, 0}}}, // Face 1
        {{{0, 1, 0}, {1, 1, 0}, {0, 0, 1}}}, // Face 2
        {{{1, 0, 1}, {0, 0, 1}, {1, 1, 0}}}, // Face 3
        {{{0, 1, 1}, {1, 0, 1}, {1, 0, 0}}}, // Face 4
        {{{2, 0, 0}, {0, 1, 0}, {0, 1, 1}}}, // Face 5
        {{{0, 1, 1}, {1, 1, 0}, {1, 0, 1}}}, // Face 6
        {{{1, 0, 0}, {1, 1, 0}, {0, 1, 0}}}, // Face 7
        {{{1, 0, 1}, {0, 1, 0}, {0, 1, 1}}}, // Face 8
        {{{2, 0, 0}, {1, 0, 1}, {0, 1, 1}}}, // Face 9
        {{{2, 0, 0}, {0, 1, 0}, {0, 0, 1}}}, // Face 10
        {{{1, 0, 0}, {0, 1, 1}, {0, 1, 0}}}, // Face 11
        {{{2, 0, 0}, {1, 0, 1}, {0, 1, 1}}}, // Face 12
        {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}}, // Face 13
        {{{0, 1, 1}, {1, 1, 0}, {1, 0, 1}}}, // Face 14
        {{{2, 0, 0}, {1, 1, 0}, {0, 0, 1}}}, // Face 15
        {{{1, 0, 0}, {1, 0, 1}, {1, 1, 0}}}, // Face 16
        {{{2, 0, 0}, {0, 1, 1}, {0, 0, 1}}}, // Face 17
        {{{0, 1, 0}, {1, 1, 0}, {1, 0, 1}}}, // Face 18
        {{{1, 0, 0}, {0, 1, 0}, {0, 1, 1}}}  // Face 19
    }};

    /// @brief Helper to find the local index of a vertex on a face's edge.
    std::int8_t find_vertex_on_face(const id_t face, const std::int8_t vertex_to_find) noexcept
    {
        const auto& vertices = face_to_vertices[+face];
        for (std::int8_t i {}; i < 3; ++i)
            if (vertices[i] == vertex_to_find)
                return i;

        return -1; // Should not happen
    }

    /// @brief Adjusts FaceIJK coordinates when a grid traversal from a HEXAGON crosses an icosahedron face boundary.
    /// @details This function handles the simpler, non-distorted case of face overage. It finds the
    ///          neighboring base cell on the new face and calculates the required rotation to translate
    ///          the IJK coordinates into the new face's system.
    /// @note    This is a low-level helper, intended to be called by a higher-level neighbor-finding algorithm
    ///          after it has detected an overage.
    /// @param[in] fijk The original FaceIJK with orientation, before the overage was detected.
    /// @param res The resolution of the traversal.
    /// @param digit The direction of movement that caused the overage.
    /// @param[out] out_fijk The resulting FaceIJK on the new face.
    static error_t adjust_hexagon_overage(const oriented_ijk& fijk, const resolution_t res, const direction_t digit,
                                          oriented_ijk& out_fijk) noexcept
    {
        // 1. Determine the base cell of the original coordinates.
        cell::base::id_t base_cell_id;
        int orientation; // We need the base cell's rotation.
        error_t err = to_base_cell_and_orientation(fijk, res, base_cell_id, orientation);
        if (err != error_t::none)
            return err;

        // This function must not be called for pentagons.
        if (cell::pentagon::check(base_cell_id))
            return error_t::pentagon; // Logic error: wrong function called.

        // 2. Find the neighboring base cell in the direction of movement.
        const cell::base::id_t new_base_cell_id = cell::base::neighbor_of(base_cell_id, digit);
        if (new_base_cell_id == cell::base::invalid_index)
            return error_t::failed; // Should not happen for a valid hexagon and direction.

        // 3. Find the rotation adjustment required to move between these two base cells.
        const auto& rotations = cell::base::rotations_60ccw(base_cell_id);
        const int new_rotation = rotations[+digit];

        // 4. Create the new FaceIJK on the new face.
        // The new face is the home face of the new base cell.
        static_cast<ijk&>(out_fijk) = home(new_base_cell_id);

        // 5. The original coordinates need to be rotated into the new system.
        out_fijk.ijk_coords = fijk.ijk_coords;
        for (int i {}; i < new_rotation; ++i)
            out_fijk.ijk_coords.rotate_60ccw();

        // 6. The overall system rotation also accumulates this change.
        out_fijk.ccw_rotations_60 = (fijk.ccw_rotations_60 + new_rotation + 6) % 6;
        return error_t::none;
    }

    error_t find_neighbor_face_ijk(const oriented_ijk& start_fijk, const resolution_t res, const direction_t dir,
                                   oriented_ijk& out_neighbor_fijk) noexcept
    {
        if ((dir == direction_t::center) || (dir == direction_t::invalid))
            return error_t::domain; // Cannot find a neighbor in the center or an invalid direction.

        // 1. Perform a naive translation in the current face's IJK system.
        out_neighbor_fijk = start_fijk;
        out_neighbor_fijk.ijk_coords += coordinate::to_ijk(dir);
        out_neighbor_fijk.ijk_coords.normalize();

        // 2. Check for "overage" - has the new coordinate spilled outside the current face's system?
        // The maximum coordinate for a given resolution defines the boundary.
        // This is equivalent to H3's `MAX_FACE_COORD`.
        const auto max_coord = 3 * unsafe_ipow<int>(7, +res);

        if ((std::abs(out_neighbor_fijk.ijk_coords.i) <= max_coord) && (std::abs(out_neighbor_fijk.ijk_coords.j) <= max_coord) &&
            (std::abs(out_neighbor_fijk.ijk_coords.k) <= max_coord))
            // No overage occurred. The neighbor is on the same face. We are done.
            return error_t::none;

        // 3. Overage occurred. The neighbor is on a new face. We must correct the coordinates.
        // The type of correction depends on whether the *origin* cell is a pentagon or a hexagon.
        const cell::base::id_t original_base_cell = to_base_cell(start_fijk, res);
        if (original_base_cell == cell::base::invalid_index)
            return error_t::cell_invalid; // Could not determine the base cell of the origin.

        if (cell::pentagon::check(original_base_cell))
            // Case A: The traversal originated from a pentagon.
            // Use the complex pentagon-specific adjustment logic.
            // This call will fix the `out_neighbor_fijk` in-place.
            return adjust_overage(out_neighbor_fijk, res, dir);

        // Case B: The traversal originated from a regular hexagon.
        // Use the simpler hexagon-to-hexagon face crossing logic.
        return adjust_hexagon_overage(start_fijk, res, dir, out_neighbor_fijk);
    }

    /// @brief H3 internal lookup table `pentagonDirectionFaces`.
    /// @details This table is critical for grid traversal across icosahedron face
    /// boundaries near pentagons. For a given pentagon (by its local 0-11 index)
    /// and a neighbor direction, it provides the `FaceIJK` coordinates on the
    /// adjacent face.
    ///
    /// The table is indexed by `[local_pentagon_index][+direction - 1]`.
    /// The direction index maps as follows:
    /// 0: k_axes, 1: j_axes, 2: jk_axes, 3: i_axes, 4: ik_axes, 5: ij_axes
    static constexpr std::array<std::array<ijk, 6u>, cell::pentagon::count> pentagon_neighbor_faces {{
        // P0 (BC 4, I-axes)
        {{{{0, 0, 1}, id_t::f4},
          {{0, 1, 0}, id_t::f3},
          {{0, 1, 0}, id_t::f2},
          {{0, 1, 0}, id_t::f1},
          {{0, 1, 0}, id_t::f0},
          {{1, 0, 0}, id_t::f5}}},
        // P1 (BC 14, I-axes)
        {{{{0, 0, 1}, id_t::f0},
          {{0, 1, 0}, id_t::f5},
          {{1, 0, 0}, id_t::f6},
          {{0, 0, 1}, id_t::f2},
          {{0, 1, 0}, id_t::f1},
          {{1, 0, 0}, id_t::f10}}},
        // P2 (BC 24, I-axes)
        {{{{0, 0, 1}, id_t::f1},
          {{0, 0, 1}, id_t::f6},
          {{1, 0, 0}, id_t::f7},
          {{0, 0, 1}, id_t::f3},
          {{0, 1, 0}, id_t::f2},
          {{1, 0, 0}, id_t::f0}}},
        // P3 (BC 38, I-axes)
        {{{{0, 0, 1}, id_t::f2},
          {{0, 0, 1}, id_t::f7},
          {{1, 0, 0}, id_t::f8},
          {{0, 0, 1}, id_t::f4},
          {{0, 1, 0}, id_t::f3},
          {{1, 0, 0}, id_t::f1}}},
        // P4 (BC 49, I-axes)
        {{{{0, 0, 1}, id_t::f3},
          {{0, 0, 1}, id_t::f8},
          {{1, 0, 0}, id_t::f9},
          {{0, 0, 1}, id_t::f0},
          {{0, 1, 0}, id_t::f4},
          {{1, 0, 0}, id_t::f2}}},
        // P5 (BC 58, K-axes)
        {{{{0, 1, 0}, id_t::f10},
          {{0, 1, 0}, id_t::f9},
          {{0, 0, 1}, id_t::f8},
          {{0, 1, 0}, id_t::f7},
          {{0, 1, 0}, id_t::f6},
          {{1, 0, 0}, id_t::f11}}},
        // P6 (BC 63, J-axes)
        {{{{0, 0, 1}, id_t::f11},
          {{0, 0, 1}, id_t::f5},
          {{1, 0, 0}, id_t::f1},
          {{1, 0, 0}, id_t::f2},
          {{1, 0, 0}, id_t::f7},
          {{1, 0, 0}, id_t::f15}}},
        // P7 (BC 72, K-axes)
        {{{{0, 1, 0}, id_t::f15},
          {{0, 0, 1}, id_t::f6},
          {{1, 0, 0}, id_t::f2},
          {{1, 0, 0}, id_t::f3},
          {{1, 0, 0}, id_t::f8},
          {{1, 0, 0}, id_t::f12}}},
        // P8 (BC 83, J-axes)
        {{{{0, 0, 1}, id_t::f12},
          {{0, 1, 0}, id_t::f7},
          {{1, 0, 0}, id_t::f3},
          {{1, 0, 0}, id_t::f4},
          {{1, 0, 0}, id_t::f9},
          {{1, 0, 0}, id_t::f16}}},
        // P9 (BC 97, K-axes)
        {{{{0, 1, 0}, id_t::f16},
          {{0, 1, 0}, id_t::f8},
          {{1, 0, 0}, id_t::f4},
          {{1, 0, 0}, id_t::f0},
          {{0, 1, 0}, id_t::f5},
          {{1, 0, 0}, id_t::f13}}},
        // P10 (BC 107, I-axes)
        {{{{1, 0, 0}, id_t::f14},
          {{0, 1, 0}, id_t::f13},
          {{0, 0, 1}, id_t::f9},
          {{0, 0, 1}, id_t::f5},
          {{1, 0, 0}, id_t::f1},
          {{1, 0, 0}, id_t::f18}}},
        // P11 (BC 117, J-axes)
        {{{{1, 0, 0}, id_t::f17},
          {{0, 0, 1}, id_t::f18},
          {{0, 0, 1}, id_t::f14},
          {{0, 0, 1}, id_t::f10},
          {{1, 0, 0}, id_t::f5},
          {{0, 1, 0}, id_t::f19}}},
    }};

    /// @brief For a given pentagon and neighbor direction, gets the corresponding FaceIJK on the adjacent face.
    /// @details This is essential for grid traversal algorithms that cross icosahedron face boundaries
    /// originating from a pentagonal cell. A pentagon has only 5 neighbors; attempting to move in the
    /// `ij_axes` direction is invalid and will return no result.
    /// @param pentagon_no The local index of the pentagon (0-11).
    /// @param direction The direction of movement away from the pentagon.
    /// @return An optional containing the `FaceIJK` of the neighbor if the direction is valid for a pentagon,
    /// otherwise `std::nullopt`.
    std::optional<ijk> get_pentagon_face_ijk(const std::uint8_t pentagon_no, const direction_t direction) noexcept
    {
        // A pentagon's local index must be in the range [0, 11].
        if (pentagon_no >= cell::pentagon::count)
            return std::nullopt;

        const auto dir_val = +direction;

        // Valid neighbor directions for a pentagon are K, J, JK, I, IK (values 1-5).
        // The `ij_axes` direction (6) is the "missing" neighbor.
        // `center` (0) and `invalid` (7) are also not neighbor directions.
        if (dir_val < +direction_t::k_axes || dir_val > +direction_t::ik_axes)
            return {};

        // The lookup table is 0-indexed by `direction - 1`.
        const auto dir_idx = dir_val - 1u;

        return pentagon_neighbor_faces[pentagon_no][dir_idx];
    }

    /// @ref _adjustOverageClassII and _adjustOverageClassIII (H3 C internal)
    error_t adjust_overage(id_t& current_face, std::int8_t& current_ccw_rotations_60, coordinate::ijk& ijk_coords_to_adjust,
                           const cell::base::id_t base_cell_id, const direction_t digit_moved) noexcept
    {
        // This function is only for handling pentagon-specific overage.
        // It should only be called by a higher-level algorithm that has already
        // determined an overage has occurred from a pentagon base cell.

        if ((digit_moved == direction_t::center) || (digit_moved == direction_t::invalid))
            return error_t::domain;

        // Fast path: No special adjustment is needed if the base cell is not a pentagon.
        // The calling function handles simple hexagon-to-hexagon face crossing.
        if (!cell::pentagon::check(base_cell_id))
            return error_t::none;

        // The H3 algorithm for pentagon distortion is complex. It involves
        // looking up pre-computed new face data and rotations.
        const auto optional_local_pent_idx = cell::pentagon::get_index(base_cell_id);
        if (!optional_local_pent_idx)
            // This should be impossible if pentagon::check passed.
            return error_t::cell_invalid;

        // Find the adjacent face by looking up the result for moving from this
        // pentagon in the specified direction.
        const std::optional<ijk> new_face_data = get(static_cast<std::uint8_t>(*optional_local_pent_idx), digit_moved);
        if (!new_face_data)
            // Attempted to move into the "missing" neighbor of a pentagon.
            return error_t::pentagon;

        // Look up the required rotation for this specific face crossing.
        // H3 has precomputed tables for this (`_pentagonCwOffset`).
        const auto [cw_offset_face, cw_offset_rot] = cell::pentagon::clockwise_offsets(base_cell_id);

        // Apply the pre-computed adjustment from the lookup tables.
        current_face = new_face_data->face;

        // The new coordinates are simply the coordinates on the new face's system.
        // The H3 tables store these as simple unit vectors.
        ijk_coords_to_adjust = new_face_data->ijk_coords;

        // Update the system's rotation.
        // A full circle is 6 rotations, so we use modulo arithmetic.
        current_ccw_rotations_60 = (current_ccw_rotations_60 + cw_offset_rot + 6) % 6;
        return error_t::none;
    }

    error_t adjust_overage(oriented_ijk& fijk, const resolution_t res, const direction_t digit) noexcept
    {
        // First, determine the base cell for the current FaceIJK coordinates.
        cell::base::id_t base_cell_id;
        int orientation; // Not used here, but required by the function

        const error_t base_cell_err = to_base_cell_and_orientation(fijk, res, base_cell_id, orientation);
        if (base_cell_err != error_t::none)
            return base_cell_err;

        // Now, delegate to the core "worker" function.
        return adjust_overage(fijk.face, fijk.ccw_rotations_60, fijk.ijk_coords, base_cell_id, digit);
    }
}
