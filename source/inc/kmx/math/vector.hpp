/// @file kmx/math/vector.hpp
#pragma once
#ifndef PCH
    #include <cassert> // For assert()
    #include <cmath>
    #include <cstdint>
#endif

namespace kmx::math
{
    template <typename T>
    struct vector2
    {
        using value_type = T;

        T x {}, y {};

        constexpr vector2() noexcept = default;
        constexpr vector2(const T x_val, const T y_val) noexcept: x {x_val}, y {y_val} {}
        constexpr vector2(const T v[2u]) noexcept: x {v[0u]}, y {v[1u]} {}

        // Member Access

        /// @brief Provides safe access to vector components by index.
        /// @param index The index of the component (0 for x, 1 for y).
        /// @return A reference to the component.
        constexpr T& operator[](const std::uint8_t index) noexcept
        {
            assert(index < 2u && "Vector index out of bounds");
            return index == 0u ? x : y;
        }

        /// @brief Provides safe const access to vector components by index.
        constexpr const T& operator[](const std::uint8_t index) const noexcept
        {
            assert(index < 2u && "Vector index out of bounds");
            return index == 0u ? x : y;
        }

        // Scalar Arithmetic Operators

        constexpr vector2& operator+=(const T scalar) noexcept
        {
            x += scalar;
            y += scalar;
            return *this;
        }

        constexpr vector2& operator-=(const T scalar) noexcept
        {
            x -= scalar;
            y -= scalar;
            return *this;
        }

        constexpr vector2& operator*=(const T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        constexpr vector2& operator/=(const T scalar) noexcept
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        // Vector Arithmetic Operators

        constexpr vector2& operator+=(const vector2& other) noexcept
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr vector2& operator-=(const vector2& other) noexcept
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }
    };

    // Non-Member Operators for vector2

    template <typename T>
    [[nodiscard]] constexpr vector2<T> operator+(vector2<T> lhs, const vector2<T>& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    template <typename T>
    [[nodiscard]] constexpr vector2<T> operator-(vector2<T> lhs, const vector2<T>& rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }

    template <typename T>
    [[nodiscard]] constexpr vector2<T> operator*(vector2<T> vec, const T scalar) noexcept
    {
        vec *= scalar;
        return vec;
    }

    template <typename T>
    [[nodiscard]] constexpr vector2<T> operator*(const T scalar, vector2<T> vec) noexcept
    {
        vec *= scalar;
        return vec;
    }

    template <typename T>
    [[nodiscard]] constexpr vector2<T> operator/(vector2<T> vec, const T scalar) noexcept
    {
        vec /= scalar;
        return vec;
    }

    // Type Aliases
    using vector2d = vector2<double>;
    using vector2f = vector2<float>;

    // ===================================================================================

    template <typename T>
    struct vector3
    {
        using value_type = T;

        T x {}, y {}, z {};

        constexpr vector3() noexcept = default;
        constexpr vector3(const T x_val, const T y_val, const T z_val) noexcept: x {x_val}, y {y_val}, z {z_val} {}
        constexpr vector3(const T v[3u]) noexcept: x {v[0u]}, y {v[1u]}, z {v[2u]} {}

        // Member Access

        constexpr T& operator[](const std::uint8_t index) noexcept
        {
            assert(index < 3u && "Vector index out of bounds");
            if (index == 0u)
                return x;
            if (index == 1u)
                return y;
            return z;
        }

        constexpr const T& operator[](const std::uint8_t index) const noexcept
        {
            assert(index < 3u && "Vector index out of bounds");
            if (index == 0u)
                return x;
            if (index == 1u)
                return y;
            return z;
        }

        // Scalar Arithmetic Operators

        constexpr vector3& operator+=(const T scalar) noexcept
        {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }

        constexpr vector3& operator-=(const T scalar) noexcept
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }

        constexpr vector3& operator*=(const T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        constexpr vector3& operator/=(const T scalar) noexcept
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        // Vector Arithmetic Operators

        constexpr vector3& operator+=(const vector3& other) noexcept
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr vector3& operator-=(const vector3& other) noexcept
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        // Vector Operations

        /// @brief Calculates the dot product of this vector with another.
        [[nodiscard]] constexpr T dot(const vector3& other) const noexcept { return x * other.x + y * other.y + z * other.z; }

        /// @brief Calculates the cross product of this vector with another.
        [[nodiscard]] constexpr vector3 cross(const vector3& other) const noexcept
        {
            return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
        }

        /// @brief Calculates the squared Euclidean norm (length or magnitude).
        /// @details This is much faster than `magnitude()` as it avoids a square root,
        //  making it ideal for distance comparisons.
        [[nodiscard]] constexpr T magnitude_sq() const noexcept { return x * x + y * y + z * z; }

        /// @brief Calculates the Euclidean norm (length or magnitude) of the vector.
        [[nodiscard]] double magnitude() const noexcept
        {
            // Use double for intermediate calculations to prevent overflow and maintain precision.
            const double dx = static_cast<double>(x);
            const double dy = static_cast<double>(y);
            const double dz = static_cast<double>(z);
            return std::sqrt(dx * dx + dy * dy + dz * dz);
        }

        /// @brief Returns a normalized version of this vector (with a magnitude of 1).
        [[nodiscard]] vector3 normalized() const noexcept
        {
            const double mag = magnitude();
            if (mag > 1e-9)
            { // Use an epsilon to avoid division by zero for very small vectors
                const auto inv_mag = static_cast<T>(1.0 / mag);
                return {x * inv_mag, y * inv_mag, z * inv_mag};
            }
            return {}; // Return a zero vector if magnitude is effectively zero
        }
    };

    // Non-Member Operators for vector3

    template <typename T>
    [[nodiscard]] constexpr vector3<T> operator+(vector3<T> lhs, const vector3<T>& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    template <typename T>
    [[nodiscard]] constexpr vector3<T> operator-(vector3<T> lhs, const vector3<T>& rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }

    template <typename T>
    [[nodiscard]] constexpr vector3<T> operator*(vector3<T> vec, const T scalar) noexcept
    {
        vec *= scalar;
        return vec;
    }

    template <typename T>
    [[nodiscard]] constexpr vector3<T> operator*(const T scalar, vector3<T> vec) noexcept
    {
        vec *= scalar;
        return vec;
    }

    template <typename T>
    [[nodiscard]] constexpr vector3<T> operator/(vector3<T> vec, const T scalar) noexcept
    {
        vec /= scalar;
        return vec;
    }

    // Type Aliases
    using vector3d = vector3<double>;
    using vector3f = vector3<float>;

} // namespace kmx::math
