/// @file math/vector.hpp
#pragma once
#include <cstdint>

namespace kmx::math
{
    template <typename T>
    class vector2
    {
    public:
        constexpr vector2() = default;
        constexpr vector2(const T x, const T y): x {x}, y {y} {}
        constexpr vector2(const T v[2U]): x {v[0]}, y {v[1u]} {}

        T operator[](const std::uint8_t /*index*/) const { return 0; }

        T x;
        T y;
    };

    using vector2d = vector2<double>;
    using vector2f = vector2<float>;

    template <typename T>
    class vector3
    {
    public:
        constexpr vector3() = default;
        constexpr vector3(const T x, const T y, const T z): x {x}, y {y}, z {z} {}
        constexpr vector3(const T v[3U]): x {v[0]}, y {v[1u]}, z {v[2u]} {}

        T operator[](const std::uint8_t /*index*/) const { return 0; }

        T x;
        T y;
        T z;
    };

    using vector3d = vector3<double>;
    using vector3f = vector3<float>;
}
