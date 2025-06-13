/// @file src/kmx/geohex/cell/base.cpp
/// @ingroup Internal
#include "kmx/geohex/cell/base.hpp"

namespace kmx::geohex::cell::base
{
    using index_array_array_t = std::array<base_id_array_t, count>;

    static constexpr index_array_array_t neighbor_data {{
        {0u, 1u, 5u, 2u, 4u, 3u, 8u},                        // base cell 0
        {1u, 7u, 6u, 9u, 0u, 3u, 2u},                        // base cell 1
        {2u, 6u, 10u, 11u, 0u, 1u, 5u},                      // base cell 2
        {3u, 13u, 1u, 7u, 4u, 12u, 0u},                      // base cell 3
        {4u, invalid_index, 15u, 8u, 3u, 0u, 12u},           // base cell 4 (pentagon)
        {5u, 2u, 18u, 10u, 8u, 0u, 16u},                     // base cell 5
        {6u, 14u, 11u, 17u, 1u, 9u, 2u},                     // base cell 6
        {7u, 21u, 9u, 19u, 3u, 13u, 1u},                     // base cell 7
        {8u, 5u, 22u, 16u, 4u, 0u, 15u},                     // base cell 8
        {9u, 19u, 14u, 20u, 1u, 7u, 6u},                     // base cell 9
        {10u, 11u, 24u, 23u, 5u, 2u, 18u},                   // base cell 10
        {11u, 17u, 23u, 25u, 2u, 6u, 10u},                   // base cell 11
        {12u, 28u, 13u, 26u, 4u, 15u, 3u},                   // base cell 12
        {13u, 26u, 21u, 29u, 3u, 12u, 7u},                   // base cell 13
        {14u, invalid_index, 17u, 27u, 9u, 20u, 6u},         // base cell 14 (pentagon)
        {15u, 22u, 28u, 31u, 4u, 8u, 12u},                   // base cell 15
        {16u, 18u, 33u, 30u, 8u, 5u, 22u},                   // base cell 16
        {17u, 11u, 14u, 6u, 35u, 25u, 27u},                  // base cell 17
        {18u, 24u, 30u, 32u, 5u, 10u, 16u},                  // base cell 18
        {19u, 34u, 20u, 36u, 7u, 21u, 9u},                   // base cell 19
        {20u, 14u, 19u, 9u, 40u, 27u, 36u},                  // base cell 20
        {21u, 38u, 19u, 34u, 13u, 29u, 7u},                  // base cell 21
        {22u, 16u, 41u, 33u, 15u, 8u, 31u},                  // base cell 22
        {23u, 24u, 11u, 10u, 39u, 37u, 25u},                 // base cell 23
        {24u, invalid_index, 32u, 37u, 10u, 23u, 18u},       // base cell 24 (pentagon)
        {25u, 23u, 17u, 11u, 45u, 39u, 35u},                 // base cell 25
        {26u, 42u, 29u, 43u, 12u, 28u, 13u},                 // base cell 26
        {27u, 40u, 35u, 46u, 14u, 20u, 17u},                 // base cell 27
        {28u, 31u, 42u, 44u, 12u, 15u, 26u},                 // base cell 28
        {29u, 43u, 38u, 47u, 13u, 26u, 21u},                 // base cell 29
        {30u, 32u, 48u, 50u, 16u, 18u, 33u},                 // base cell 30
        {31u, 41u, 44u, 53u, 15u, 22u, 28u},                 // base cell 31
        {32u, 30u, 24u, 18u, 52u, 50u, 37u},                 // base cell 32
        {33u, 30u, 49u, 48u, 22u, 16u, 41u},                 // base cell 33
        {34u, 19u, 38u, 21u, 54u, 36u, 51u},                 // base cell 34
        {35u, 46u, 45u, 56u, 17u, 27u, 25u},                 // base cell 35
        {36u, 20u, 34u, 19u, 55u, 40u, 54u},                 // base cell 36
        {37u, 39u, 52u, 57u, 24u, 23u, 32u},                 // base cell 37
        {38u, invalid_index, 34u, 51u, 29u, 47u, 21u},       // base cell 38 (pentagon)
        {39u, 37u, 25u, 23u, 59u, 57u, 45u},                 // base cell 39
        {40u, 27u, 36u, 20u, 60u, 46u, 55u},                 // base cell 40
        {41u, 49u, 53u, 61u, 22u, 33u, 31u},                 // base cell 41
        {42u, 58u, 43u, 62u, 28u, 44u, 26u},                 // base cell 42
        {43u, 62u, 47u, 64u, 26u, 42u, 29u},                 // base cell 43
        {44u, 53u, 58u, 65u, 28u, 31u, 42u},                 // base cell 44
        {45u, 39u, 35u, 25u, 63u, 59u, 56u},                 // base cell 45
        {46u, 60u, 56u, 68u, 27u, 40u, 35u},                 // base cell 46
        {47u, 38u, 43u, 29u, 69u, 51u, 64u},                 // base cell 47
        {48u, 49u, 30u, 33u, 67u, 66u, 50u},                 // base cell 48
        {49u, invalid_index, 61u, 66u, 33u, 48u, 41u},       // base cell 49 (pentagon)
        {50u, 48u, 32u, 30u, 70u, 67u, 52u},                 // base cell 50
        {51u, 69u, 54u, 71u, 38u, 47u, 34u},                 // base cell 51
        {52u, 57u, 70u, 74u, 32u, 37u, 50u},                 // base cell 52
        {53u, 61u, 65u, 75u, 31u, 41u, 44u},                 // base cell 53
        {54u, 71u, 55u, 73u, 34u, 51u, 36u},                 // base cell 54
        {55u, 40u, 54u, 36u, 72u, 60u, 73u},                 // base cell 55
        {56u, 68u, 63u, 77u, 35u, 46u, 45u},                 // base cell 56
        {57u, 59u, 74u, 78u, 37u, 39u, 52u},                 // base cell 57
        {58u, invalid_index, 62u, 76u, 44u, 65u, 42u},       // base cell 58 (pentagon)
        {59u, 63u, 78u, 79u, 39u, 45u, 57u},                 // base cell 59
        {60u, 72u, 68u, 80u, 40u, 55u, 46u},                 // base cell 60
        {61u, 53u, 49u, 41u, 81u, 75u, 66u},                 // base cell 61
        {62u, 43u, 58u, 42u, 82u, 64u, 76u},                 // base cell 62
        {63u, invalid_index, 56u, 45u, 79u, 59u, 77u},       // base cell 63 (pentagon)
        {64u, 47u, 62u, 43u, 84u, 69u, 82u},                 // base cell 64
        {65u, 58u, 53u, 44u, 86u, 76u, 75u},                 // base cell 65
        {66u, 67u, 81u, 85u, 49u, 48u, 61u},                 // base cell 66
        {67u, 66u, 50u, 48u, 87u, 85u, 70u},                 // base cell 67
        {68u, 56u, 60u, 46u, 90u, 77u, 80u},                 // base cell 68
        {69u, 51u, 64u, 47u, 89u, 71u, 84u},                 // base cell 69
        {70u, 67u, 52u, 50u, 83u, 87u, 74u},                 // base cell 70
        {71u, 89u, 73u, 91u, 51u, 69u, 54u},                 // base cell 71
        {72u, invalid_index, 73u, 55u, 80u, 60u, 88u},       // base cell 72 (pentagon)
        {73u, 91u, 72u, 88u, 54u, 71u, 55u},                 // base cell 73
        {74u, 78u, 83u, 92u, 52u, 57u, 70u},                 // base cell 74
        {75u, 65u, 61u, 53u, 94u, 86u, 81u},                 // base cell 75
        {76u, 86u, 82u, 96u, 58u, 65u, 62u},                 // base cell 76
        {77u, 63u, 68u, 56u, 93u, 79u, 90u},                 // base cell 77
        {78u, 74u, 59u, 57u, 95u, 92u, 79u},                 // base cell 78
        {79u, 78u, 63u, 59u, 93u, 95u, 77u},                 // base cell 79
        {80u, 68u, 72u, 60u, 99u, 90u, 88u},                 // base cell 80
        {81u, 85u, 94u, 101u, 61u, 66u, 75u},                // base cell 81
        {82u, 96u, 84u, 98u, 62u, 76u, 64u},                 // base cell 82
        {83u, invalid_index, 74u, 70u, 100u, 87u, 92u},      // base cell 83 (pentagon)
        {84u, 69u, 82u, 64u, 97u, 89u, 98u},                 // base cell 84
        {85u, 87u, 101u, 102u, 66u, 67u, 81u},               // base cell 85
        {86u, 76u, 75u, 65u, 104u, 96u, 94u},                // base cell 86
        {87u, 83u, 102u, 100u, 67u, 70u, 85u},               // base cell 87
        {88u, 72u, 91u, 73u, 99u, 80u, 105u},                // base cell 88
        {89u, 97u, 91u, 103u, 69u, 84u, 71u},                // base cell 89
        {90u, 77u, 80u, 68u, 106u, 93u, 99u},                // base cell 90
        {91u, 73u, 89u, 71u, 105u, 88u, 103u},               // base cell 91
        {92u, 83u, 78u, 74u, 108u, 100u, 95u},               // base cell 92
        {93u, 79u, 90u, 77u, 109u, 95u, 106u},               // base cell 93
        {94u, 86u, 81u, 75u, 107u, 104u, 101u},              // base cell 94
        {95u, 92u, 79u, 78u, 109u, 108u, 93u},               // base cell 95
        {96u, 104u, 98u, 110u, 76u, 86u, 82u},               // base cell 96
        {97u, invalid_index, 98u, 84u, 103u, 89u, 111u},     // base cell 97 (pentagon)
        {98u, 110u, 97u, 111u, 82u, 96u, 84u},               // base cell 98
        {99u, 80u, 105u, 88u, 106u, 90u, 113u},              // base cell 99
        {100u, 102u, 83u, 87u, 108u, 114u, 92u},             // base cell 100
        {101u, 102u, 107u, 112u, 81u, 85u, 94u},             // base cell 101
        {102u, 101u, 87u, 85u, 114u, 112u, 100u},            // base cell 102
        {103u, 91u, 97u, 89u, 116u, 105u, 111u},             // base cell 103
        {104u, 107u, 110u, 115u, 86u, 94u, 96u},             // base cell 104
        {105u, 88u, 103u, 91u, 113u, 99u, 116u},             // base cell 105
        {106u, 93u, 99u, 90u, 117u, 109u, 113u},             // base cell 106
        {107u, invalid_index, 101u, 94u, 115u, 104u, 112u},  // base cell 107 (pentagon)
        {108u, 100u, 95u, 92u, 118u, 114u, 109u},            // base cell 108
        {109u, 108u, 93u, 95u, 117u, 118u, 106u},            // base cell 109
        {110u, 98u, 104u, 96u, 119u, 111u, 115u},            // base cell 110
        {111u, 97u, 110u, 98u, 116u, 103u, 119u},            // base cell 111
        {112u, 107u, 102u, 101u, 120u, 115u, 114u},          // base cell 112
        {113u, 99u, 116u, 105u, 117u, 106u, 121u},           // base cell 113
        {114u, 112u, 100u, 102u, 118u, 120u, 108u},          // base cell 114
        {115u, 110u, 107u, 104u, 120u, 119u, 112u},          // base cell 115
        {116u, 103u, 119u, 111u, 113u, 105u, 121u},          // base cell 116
        {117u, invalid_index, 109u, 118u, 113u, 121u, 106u}, // base cell 117 (pentagon)
        {118u, 120u, 108u, 114u, 117u, 121u, 109u},          // base cell 118
        {119u, 111u, 115u, 110u, 121u, 116u, 120u},          // base cell 119
        {120u, 115u, 114u, 112u, 121u, 119u, 118u},          // base cell 120
        {121u, 116u, 120u, 119u, 117u, 113u, 118u},          // base cell 121
    }};

    id_t neighbor_of(const id_t item, const direction_t direction)
    {
        return neighbor_data[item][+direction];
    }

    direction_t direction_between(const id_t origin, const id_t neightbor)
    {
        for (std::uint8_t i {}; i != direction_count; ++i)
        {
            const auto direction = static_cast<direction_t>(i);
            const auto index = neighbor_of(origin, direction);
            if (index == neightbor)
                return direction;
        }

        return direction_t::invalid;
    }

    static constexpr std::array<rotations_60ccw_per_direction_array, 27u> distinct_rotation_data {{
        {0, 5, 0, 0, 1, 5, 1},  // Index 0
        {0, 0, 1, 0, 1, 0, 1},  // Index 1
        {0, 0, 0, 0, 0, 5, 0},  // Index 2
        {0, 5, 0, 0, 2, 5, 1},  // Index 3
        {0, -1, 1, 0, 3, 4, 2}, // Index 4 (pentagon)
        {0, 0, 0, 3, 5, 5, 0},  // Index 5
        {0, 5, 0, 0, 0, 5, 1},  // Index 6
        {0, 0, 1, 3, 0, 0, 1},  // Index 7
        {0, 3, 3, 3, 0, 0, 0},  // Index 8
        {0, 5, 0, 0, 3, 5, 1},  // Index 9
        {0, -1, 3, 0, 5, 2, 0}, // Index 10 (pentagon)
        {0, 5, 0, 0, 4, 5, 1},  // Index 11
        {0, 3, 3, 3, 3, 0, 3},  // Index 12
        {0, 3, 3, 3, 0, 3, 0},  // Index 13
        {0, 0, 0, 3, 0, 0, 3},  // Index 14
        {0, 3, 0, 0, 0, 3, 3},  // Index 15
        {0, 0, 3, 0, 3, 0, 3},  // Index 16
        {0, 3, 0, 0, 3, 3, 0},  // Index 17
        {0, 0, 3, 0, 0, 3, 3},  // Index 18
        {0, 3, 3, 3, 0, 0, 3},  // Index 19
        {0, 3, 3, 3, 3, 3, 0},  // Index 20
        {0, 0, 0, 3, 0, 5, 0},  // Index 21
        {0, 0, 1, 3, 1, 0, 1},  // Index 22
        {0, 0, 0, 0, 0, 0, 1},  // Index 23
        {0, 5, 0, 0, 5, 5, 0},  // Index 24
        {0, 0, 1, 0, 3, 5, 1},  // Index 25
        {0, 0, 1, 0, 0, 5, 1}   // Index 26
    }};

    static constexpr std::array<std::uint8_t, 122u> rotation_index_map {{
        0u,  1u,  2u,  3u,  4u,  1u,  5u,  2u,  6u,  7u,  // Maps rows 0-9
        7u,  8u,  9u,  1u,  10u, 11u, 2u,  12u, 5u,  8u,  // Maps rows 10-19
        13u, 5u,  1u,  13u, 10u, 14u, 2u,  15u, 1u,  7u,  // Maps rows 20-29
        8u,  2u,  12u, 7u,  12u, 16u, 14u, 15u, 10u, 17u, // Maps rows 30-39
        17u, 5u,  5u,  8u,  7u,  18u, 13u, 13u, 10u, 14u, // Maps rows 40-49
        15u, 16u, 8u,  16u, 18u, 19u, 13u, 10u, 20u, 20u, // Maps rows 50-59
        12u, 12u, 10u, 14u, 13u, 15u, 17u, 8u,  17u, 18u, // Maps rows 60-69
        13u, 10u, 19u, 19u, 14u, 15u, 21u, 8u,  22u, 22u, // Maps rows 70-79
        16u, 16u, 10u, 18u, 13u, 17u, 20u, 21u, 20u, 23u, // Maps rows 80-89
        8u,  21u, 24u, 18u, 23u, 13u, 10u, 19u, 24u, 22u, // Maps rows 90-99
        19u, 8u,  22u, 20u, 23u, 25u, 10u, 24u, 11u, 8u,  // Maps rows 100-109
        21u, 21u, 3u,  23u, 22u, 24u, 4u,  26u, 23u, 24u, // Maps rows 110-119
        0u,  26u                                          // Maps rows 120-121
    }};

    const rotations_60ccw_per_direction_array& rotations_60ccw(const id_t base_cell_id) noexcept
    {
        return distinct_rotation_data[rotation_index_map[base_cell_id]];
    }

}
