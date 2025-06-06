/// @file geohex/base.cpp
#include "kmx/geohex/base.hpp"
#include <array>

namespace kmx::geohex
{
    double scaling_factor(const resolution_t resolution)
    {
        /// @ref UNIT_VEC_SCALES
        static constexpr std::array<double, resolution_count> data {
            0.3629383840444699,  // res 0
            0.13714663375085900, // res 1
            0.05182828247323050, // res 2
            0.01958407039253940, // res 3
            0.00740020964420035, // res 4
            0.00279647870734598, // res 5
            0.00105675958568959, // res 6
            0.00039932674915862, // res 7
            0.00015090046899810, // res 8
            0.00005702422364090, // res 9    (5.702422364090e-5)
            0.00002154952037130, // res 10   (2.154952037130e-5)
            0.00000814294680010, // res 11   (8.14294680010e-6)
            0.00000307721840800, // res 12   (3.07721840800e-6)
            0.00000116281431600, // res 13   (1.16281431600e-6)
            0.00000043942111800, // res 14   (4.3942111800e-7)
            0.00000016605308800  // res 15   (1.6605308800e-7)
        };

        return data[+resolution];
    }
}
