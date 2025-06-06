/// @file geohex/icosahedron.cpp
#include "kmx/geohex/icosahedron/face.hpp"
#include "kmx/geohex/cell.hpp"
#include "kmx/geohex/cell/boundary.hpp"

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

    id_t of(const cell::base::id_t base_cell_id)
    {
        return face_data[base_cell_id];
    }

    bool is_cw_offset(const cell::base::id_t base_cell_id, const id_t face) noexcept
    {
        const auto offsets = cell::base::clockwise_offsets(base_cell_id);
        const auto face_id = static_cast<cell::base::clockwise_offset_t>(+face);
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

    error_t get_intersected(const index index, std::span<no_t>& out_faces) noexcept
    {
        if (out_faces.empty())
            return error_t::memory_bounds; // Output span must have some capacity

        if (!index.is_valid())
            return error_t::cell_invalid;

        // Initialize output with an invalid face number or rely on caller to check count
        for (auto& f: out_faces)
            f = count + 1u;

        // 1. Get the cell's boundary vertices.
        std::array<gis::wgs84::coordinate, cell::boundary::max_vertices> boundary_data;
        std::span<gis::wgs84::coordinate> boundary_data_span {boundary_data.begin(), boundary_data.end()};
        const auto boundary_err = cell::boundary::get(index, boundary_data_span);
        if (boundary_err != error_t::none)
            return boundary_err; // Other errors like cell_invalid if get_boundary re-validates

        if (boundary_data_span.empty())
            // This might happen if get_boundary itself couldn't produce vertices (e.g. not_implemented deeper)
            // Or if it's a point-like cell (not standard H3 but theoretically)
            return error_t::failed; // Or none, if an empty cell has no faces (H3 cells always have area)

        constexpr std::uint8_t MAX_CELL_BOUNDARY_VERTICES = 10u;

        no_t faces_found[MAX_CELL_BOUNDARY_VERTICES + 1u]; // Max possible unique faces from vertices + center
        std::uint8_t num_unique_faces {};

        // 2. For each vertex, find the face it's on.
        for (std::size_t i {}; i < boundary_data_span.size(); ++i)
        {
            id_t vert_face = icosahedron::face::from(boundary_data_span[i]);
            no_t vert_face_no = static_cast<no_t>(+vert_face);

            // Add to list if unique
            bool found {};
            for (std::uint8_t j = 0; j < num_unique_faces; ++j)
                if (faces_found[j] == vert_face_no)
                {
                    found = true;
                    break;
                }

            if (!found)
            {
                if (num_unique_faces < (sizeof(faces_found) / sizeof(faces_found)))
                    // bounds check
                    faces_found[num_unique_faces++] = vert_face_no;
                else
                    // Should not happen if MAX_CELL_BOUNDARY_VERTICES is large enough
                    // and max_face_intersection_count is respected by output span.
                    return error_t::failed; // Internal error / too many unique faces from vertices
            }
        }

        // 3. Also check the cell center, as vertices alone might not capture the face
        //    for a cell entirely within one face but near an edge.
        gis::wgs84::coordinate cell_center_geo;
        error_t center_err = cell::to_wgs84(index, cell_center_geo);
        if (center_err != error_t::none)
            return center_err;

        id_t center_face = icosahedron::face::from(cell_center_geo);
        no_t center_face_no = static_cast<no_t>(+center_face);
        bool center_face_is_new = true;
        for (std::uint8_t j {}; j < num_unique_faces; ++j)
            if (faces_found[j] == center_face_no)
            {
                center_face_is_new = false;
                break;
            }

        if (center_face_is_new)
        {
            if (num_unique_faces < (sizeof(faces_found) / sizeof(faces_found)))
                faces_found[num_unique_faces++] = center_face_no;
            else
                return error_t::failed; // Internal error
        }

        // 4. Copy unique faces to output span, respecting its size.
        //    The number of faces should not exceed max_face_intersection_count(index).
        const auto max_allowed = std::min(static_cast<std::uint8_t>(out_faces.size()), index.max_face_intersection_count());
        const auto count_to_write = std::min(num_unique_faces, max_allowed);
        for (std::uint8_t i {}; i < count_to_write; ++i)
            out_faces[i] = faces_found[i];

        // If out_faces was larger than count_to_write, remaining elements keep sentinel.

        // The H3 C API for `getIcosahedronFaces` doesn't explicitly return the count of faces found.
        // It expects `out` to be an array of size `maxFaceCount()`, and it fills it.
        // The caller then iterates through `out` until it finds a sentinel or up to `maxFaceCount()`.
        // Our span-based approach is a bit different. If precise count is needed,
        // the function signature might need to change to return `num_unique_faces_written`.
        // For now, caller can iterate `out_faces` up to `count_to_write` or until sentinel.

        return error_t::none;
    }

    id_t from(const gis::wgs84::coordinate& coord) noexcept
    {
        // TODO implement
        return id_t::f0;
    }

    /// @ref _h3ToFaceIjk (H3 C internal)
    /// @brief Converts an H3 cell index to its corresponding FaceIJK representation.
    /// @param index The H3 cell index.
    /// @param out Output: The FaceIJK representation.
    /// @return error_t::none on success, or an error code.
    error_t from(const index index, ijk& out) noexcept
    {
        // TODO implement
        return error_t::none;
    }

    error_t to_geo(const ijk& fijk, const resolution_t res, gis::wgs84::coordinate& out_coord) noexcept
    {
        // TODO implement
        return error_t::none;
    }

    /// @ref _geoToFaceIjk (H3 C internal)
    /// @brief Converts geographic WGS84 coordinates (radians) to FaceIJK representation.
    /// @param geo_coord Geographic coordinates in radians.
    /// @param res The target H3 resolution.
    /// @param out_fijk Output: The FaceIJK representation.
    /// @return error_t::none on success.
    error_t to_ijk(const gis::wgs84::coordinate& coord, const resolution_t res, ijk& out_fijk) noexcept
    {
        // TODO implement
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
                    return icosahedron::face::ijk {std::get<0>(source), std::get<1>(source)};
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
}
