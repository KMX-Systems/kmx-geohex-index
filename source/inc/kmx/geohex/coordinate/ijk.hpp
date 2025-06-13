/// @file inc/kmx/geohex/coordinate/ijk.hpp
/// @ingroup Internal
/// @brief Defines the 3D (IJK) hexagonal cube coordinate class and related utilities.
#pragma once
#ifndef PCH
    #include <kmx/geohex/base.hpp>
    #include <kmx/geohex/coordinate/ij.hpp>
    #include <kmx/geohex/utils.hpp>
    #include <kmx/math/vector.hpp>
    #include <span>
    #include <tuple>
    #include <vector>
#endif

namespace kmx::geohex::coordinate
{
    using vector2 = kmx::math::vector2d;
    using index_as_tuple = std::tuple<int, int, int>;

    /// @brief Represents hexagonal grid coordinates in the IJK cube coordinate system.
    /// @details In this system, i + j + k = 0 always holds for valid coordinates.
    /// This property simplifies many grid algorithms like rotation and neighbor finding.
    /// @ref CoordIJK
    class ijk: public ij
    {
    public:
        // Type aliases are self-documenting.
        using span = std::span<ijk>;
        using vector = std::vector<ijk>;

        // Defaulted special members are idiomatic and self-explanatory.
        constexpr ijk() noexcept = default;
        constexpr ijk(const ijk&) noexcept = default;
        constexpr ijk(ijk&&) noexcept = default;

        /// @brief Constructs an IJK coordinate from its three integer components.
        constexpr ijk(const value i, const value j, const value k = 0) noexcept: ij(i, j), k(k) {}

        /// @brief Constructs an IJK coordinate from a tuple of values.
        template <typename T>
        constexpr ijk(const std::tuple<T, T, T>& item): ij(std::get<0>(item), std::get<1u>(item)), k(std::get<2u>(item))
        {
        }

        /// @brief Constructs an IJK coordinate from a C-style array.
        constexpr ijk(const value data[3u]) noexcept: ij(data), k(data[2u]) {}

        /// @brief Constructs an IJK coordinate from an IJ coordinate pair and a k value.
        constexpr ijk(const ij& ij_item, const value k = 0) noexcept: ij(ij_item), k(k) {}

        /// @brief Creates an IJK coordinate by rounding floating-point cube coordinates to the nearest integer center.
        /// @details This is a factory method, as its logic is non-trivial.
        /// @ref _ijkRound
        [[nodiscard]] static ijk from_cube_round(double i, double j, double k) noexcept;

        /// @brief Calculates the 2D Cartesian coordinates of the hexagon's center.
        /// @ref _ijkToHex2d
        [[nodiscard]] vector2 center() const noexcept;

        // Idiomatic operators are self-documenting.
        [[nodiscard]] ijk operator+(const ijk& item) const noexcept;
        [[nodiscard]] ijk operator-(const ijk& item) const noexcept;
        [[nodiscard]] ijk operator*(int factor) const noexcept;

        constexpr ijk& operator=(const ijk&) noexcept = default;
        constexpr ijk& operator=(ijk&&) noexcept = default;
        constexpr bool operator==(const ijk&) const noexcept = default;

        /// @brief Checks if the IJK coordinates represent the origin (0, 0, 0).
        [[nodiscard]] constexpr bool is_origin() const noexcept { return i == 0 && j == 0 && k == 0; }

        /// @brief Scales this IJK coordinate in-place by a constant factor.
        /// @ref _ijkScale
        void scale(int factor) noexcept;

        void operator+=(const ijk& item) noexcept;
        void operator-=(const ijk& item) noexcept;
        void operator*=(int factor) noexcept { scale(factor); }

        /// @brief Adjusts IJK coordinates to the nearest valid cube coordinate center.
        /// @details Ensures that i + j + k = 0.
        /// @ref _ijkNormalize
        void normalize() noexcept;

        /// @brief Converts a canonical unit IJK vector to its corresponding direction digit (0-6).
        /// @details This function normalizes a copy of the coordinate and then finds which of the
        /// 7 canonical direction vectors it matches. It will return `direction_t::invalid`
        /// for non-unit vectors.
        /// @ref _unitIjkToDigit
        [[nodiscard]] direction_t to_digit() const noexcept;

        /// @brief Moves coordinates to the parent cell in a Class II resolution grid (aperture 7).
        /// @ref _upAp7
        void up_ap7() noexcept;

        /// @brief Moves coordinates to the parent cell in a Class III resolution grid (aperture 7, rotated).
        /// @ref _upAp7r
        void up_ap7r() noexcept;

        /// @brief Moves coordinates to the child cell center in a Class II grid.
        /// @ref _downAp7
        void down_ap7() noexcept;

        /// @brief Moves coordinates to the child cell center in a Class III grid.
        /// @ref _downAp7r
        void down_ap7r() noexcept;

        /// @brief Returns a copy of this coordinate moved to a finer resolution grid.
        [[nodiscard]] ijk down_ap7(bool is_class_3) const noexcept;

        /// @brief Returns a copy of this coordinate moved to a coarser resolution grid.
        [[nodiscard]] ijk up_ap7_copy(bool is_class_3) const noexcept;

        /// @brief Moves this coordinate to a neighboring cell in a given direction.
        /// @ref _ijkNeighbor
        void to_neighbor(direction_t digit) noexcept;

        /// @brief Returns the neighboring cell's coordinates in a given direction.
        [[nodiscard]] ijk neighbor(direction_t digit) const noexcept;

        /// @brief Rotates the coordinates 60 degrees counter-clockwise around the origin.
        /// @ref _ijkRotate60ccw
        void rotate_60ccw() noexcept;

        /// @brief Rotates the coordinates 60 degrees clockwise around the origin.
        /// @ref _ijkRotate60cw
        void rotate_60cw() noexcept;

        /// @brief Calculates the grid distance between two IJK coordinates.
        /// @ref ijkDistance
        [[nodiscard]] int distance_to(const ijk& b) const noexcept;

        /// @brief Determines the leading non-zero direction digit for this coordinate at a given resolution.
        /// @ref _h3LeadingNonZeroDigit
        [[nodiscard]] direction_t leading_digit(resolution_t res) const noexcept;

        value k;
    };

    /// @brief Creates a unit IJK vector for a given direction.
    /// @ref _neighbor
    constexpr ijk to_ijk(const direction_t direction) noexcept
    {
        // This table maps directions to their corresponding unit vectors in IJK space.
        static constexpr std::array<ijk, direction_count> data {{
            {0, 0, 0}, // 0: Center
            {0, 0, 1}, // 1: K
            {0, 1, 0}, // 2: J
            {0, 1, 1}, // 3: JK
            {1, 0, 0}, // 4: I
            {1, 0, 1}, // 5: IK
            {1, 1, 0}  // 6: IJ
        }};

        // Adding a bounds check for safety in a constexpr context
        if (+direction >= direction_count)
            return {};

        return data[+direction];
    }

    /// @brief Converts IJK coordinates to a 2D Cartesian vector (axial coordinates).
    /// @ref _ijkToHex2d
    template <typename T>
    constexpr math::vector2<T> to_vec2(const ijk& coord) noexcept
    {
        const auto v = static_cast<T>(coord.j - coord.k);
        return {static_cast<T>(coord.i - coord.k) - T(0.5) * v, v * sqrt3_2};
    }
}
namespace std
{
    /// @brief Hash specialization for ijk coordinates, used for map keys.
    template <>
    struct hash<kmx::geohex::coordinate::ijk>
    {
        std::size_t operator()(const kmx::geohex::coordinate::ijk& c) const noexcept
        {
#ifdef KMX_GEOHEX_SIMPLE_HASH
            const std::size_t h1 = std::hash<decltype(c.i)> {}(c.i);
            const std::size_t h2 = std::hash<decltype(c.j)> {}(c.j);
            const std::size_t h3 = std::hash<decltype(c.k)> {}(c.k);
            // Combine hashes using a standard technique to reduce collisions.
            return h1 ^ (h2 << 1u) ^ (h3 << 2u);
#else
            std::size_t seed {};
            kmx::hash_combine(seed, c);
            return seed;
#endif
        }
    };
}
