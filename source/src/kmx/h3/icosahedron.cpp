/// @file h3/icosahedron.cpp
#include "kmx/h3/icosahedron.hpp"
#include "kmx/h3/cell/base.hpp"

namespace kmx::h3::icosahedron
{
    static constexpr std::array<face_t, cell::base::count> face_data {
        face_t::f1,  // base cell 0
        face_t::f2,  // base cell 1
        face_t::f1,  // base cell 2
        face_t::f2,  // base cell 3
        face_t::f0,  // base cell 4
        face_t::f1,  // base cell 5
        face_t::f1,  // base cell 6
        face_t::f2,  // base cell 7
        face_t::f0,  // base cell 8
        face_t::f2,  // base cell 9
        face_t::f1,  // base cell 10
        face_t::f1,  // base cell 11
        face_t::f3,  // base cell 12
        face_t::f3,  // base cell 13
        face_t::f11, // base cell 14
        face_t::f4,  // base cell 15
        face_t::f0,  // base cell 16
        face_t::f6,  // base cell 17
        face_t::f0,  // base cell 18
        face_t::f2,  // base cell 19
        face_t::f7,  // base cell 20
        face_t::f2,  // base cell 21
        face_t::f0,  // base cell 22
        face_t::f6,  // base cell 23
        face_t::f10, // base cell 24
        face_t::f6,  // base cell 25
        face_t::f3,  // base cell 26
        face_t::f11, // base cell 27
        face_t::f4,  // base cell 28
        face_t::f3,  // base cell 29
        face_t::f0,  // base cell 30
        face_t::f4,  // base cell 31
        face_t::f5,  // base cell 32
        face_t::f0,  // base cell 33
        face_t::f7,  // base cell 34
        face_t::f11, // base cell 35
        face_t::f7,  // base cell 36
        face_t::f10, // base cell 37
        face_t::f12, // base cell 38
        face_t::f6,  // base cell 39
        face_t::f7,  // base cell 40
        face_t::f4,  // base cell 41
        face_t::f3,  // base cell 42
        face_t::f3,  // base cell 43
        face_t::f4,  // base cell 44
        face_t::f6,  // base cell 45
        face_t::f11, // base cell 46
        face_t::f8,  // base cell 47
        face_t::f5,  // base cell 48
        face_t::f14, // base cell 49
        face_t::f5,  // base cell 50
        face_t::f12, // base cell 51
        face_t::f10, // base cell 52
        face_t::f4,  // base cell 53
        face_t::f12, // base cell 54
        face_t::f7,  // base cell 55
        face_t::f11, // base cell 56
        face_t::f10, // base cell 57
        face_t::f13, // base cell 58
        face_t::f10, // base cell 59
        face_t::f11, // base cell 60
        face_t::f9,  // base cell 61
        face_t::f8,  // base cell 62
        face_t::f6,  // base cell 63
        face_t::f8,  // base cell 64
        face_t::f9,  // base cell 65
        face_t::f14, // base cell 66
        face_t::f5,  // base cell 67
        face_t::f16, // base cell 68
        face_t::f8,  // base cell 69
        face_t::f5,  // base cell 70
        face_t::f12, // base cell 71
        face_t::f7,  // base cell 72
        face_t::f12, // base cell 73
        face_t::f10, // base cell 74
        face_t::f9,  // base cell 75
        face_t::f13, // base cell 76
        face_t::f16, // base cell 77
        face_t::f15, // base cell 78
        face_t::f15, // base cell 79
        face_t::f16, // base cell 80
        face_t::f14, // base cell 81
        face_t::f13, // base cell 82
        face_t::f5,  // base cell 83
        face_t::f8,  // base cell 84
        face_t::f14, // base cell 85
        face_t::f9,  // base cell 86
        face_t::f14, // base cell 87
        face_t::f17, // base cell 88
        face_t::f12, // base cell 89
        face_t::f16, // base cell 90
        face_t::f17, // base cell 91
        face_t::f15, // base cell 92
        face_t::f16, // base cell 93
        face_t::f9,  // base cell 94
        face_t::f15, // base cell 95
        face_t::f13, // base cell 96
        face_t::f8,  // base cell 97
        face_t::f13, // base cell 98
        face_t::f17, // base cell 99
        face_t::f19, // base cell 100
        face_t::f14, // base cell 101
        face_t::f19, // base cell 102
        face_t::f17, // base cell 103
        face_t::f13, // base cell 104
        face_t::f17, // base cell 105
        face_t::f16, // base cell 106
        face_t::f9,  // base cell 107
        face_t::f15, // base cell 108
        face_t::f15, // base cell 109
        face_t::f18, // base cell 110
        face_t::f18, // base cell 111
        face_t::f19, // base cell 112
        face_t::f17, // base cell 113
        face_t::f19, // base cell 114
        face_t::f18, // base cell 115
        face_t::f18, // base cell 116
        face_t::f19, // base cell 117
        face_t::f19, // base cell 118
        face_t::f18, // base cell 119
        face_t::f19, // base cell 120
        face_t::f18, // base cell 121
    };

    face_t face_of(const cell::base::id_t base_cell_id)
    {
        return face_data[base_cell_id];
    }

    bool is_cw_offset(const cell::base::id_t base_cell_id, const face_t face)
    {
        const auto offsets = cell::base::clockwise_offsets(base_cell_id);
        const auto face_id = static_cast<cell::base::clockwise_offset_t>(+face);
        return (offsets.first == face_id) || (offsets.second == face_id);
    }

    /// @brief Icosahedron face centers in x/y/z on the unit sphere.
    static constexpr std::array<vector3, face_count> face_center_point {{
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

    vector3 center_point(const face_t face) noexcept
    {
        return face_center_point[+face];
    }

    static constexpr std::array<wgs_coord, face_count> face_center_wgs {{
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

    wgs_coord center_wgs(const face_t face) noexcept
    {
        return face_center_wgs[+face];
    }

    void get_intersected_faces(const index /*index*/, std::span<face_no_t>& /*output*/) noexcept
    {
        // TODO
    }
}
