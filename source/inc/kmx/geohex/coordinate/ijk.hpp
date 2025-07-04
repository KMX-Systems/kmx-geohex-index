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
    // Forward-declare or include vector types as needed.
    using vector2 = kmx::math::vector2d;
    using index_as_tuple = std::tuple<int, int, int>;

    /// @brief Represents hexagonal grid coordinates in the IJK cube coordinate system.
    /// @details In this system, `i + j + k = 0` always holds for valid coordinates.
    /// This property simplifies many grid algorithms like rotation and neighbor finding.
    /// The class inherits the `i` and `j` components from the `ij` base struct.
    /// @ref CoordIJK
    class ijk: public ij
    {
    public:
        /// @brief A type alias for a dynamic-size collection of `ijk` coordinates.
        using vector = std::vector<ijk>;
        /// @brief A type alias for a non-owning view of a sequence of `ijk` coordinates.
        using span = std::span<ijk>;

        /// @brief Default constructor. Initializes all components to zero {0, 0, 0}.
        constexpr ijk() noexcept = default;

        /// @brief Copy constructor.
        constexpr ijk(const ijk&) noexcept = default;

        /// @brief Move constructor.
        constexpr ijk(ijk&&) noexcept = default;

        /// @brief Constructs an IJK coordinate from its three integer components.
        /// @param i The 'i' component of the cube coordinate.
        /// @param j The 'j' component of the cube coordinate.
        /// @param k The 'k' component of the cube coordinate. If omitted, it is calculated
        ///          as `-i - j` to satisfy the cube coordinate invariant, although it defaults to 0.
        ///          It is the caller's responsibility to ensure the invariant holds.
        constexpr ijk(const value i, const value j, const value k = 0) noexcept: ij(i, j), k(k) {}

        /// @brief Constructs an IJK coordinate from a `std::tuple` of three values.
        template <typename T>
        constexpr ijk(const std::tuple<T, T, T>& item): ij(std::get<0>(item), std::get<1u>(item)), k(std::get<2u>(item))
        {
        }

        /// @brief Constructs an IJK coordinate from a C-style array of three values.
        /// @param data A C-style array where `data[0]=i`, `data[1]=j`, `data[2]=k`.
        constexpr ijk(const value data[3u]) noexcept: ij(data), k(data[2u]) {}

        /// @brief Constructs an IJK coordinate from an `ij` axial coordinate pair.
        /// @param ij_item The `ij` base object.
        /// @param k The 'k' component. Defaults to 0. The invariant `i+j+k=0` is not enforced.
        constexpr ijk(const ij& ij_item, const value k = 0) noexcept: ij(ij_item), k(k) {}

        /// @brief Creates an IJK coordinate by rounding floating-point cube coordinates.
        /// @details This is a key algorithm for converting from continuous space (like a
        ///          geographic projection) to the discrete hexagonal grid. It rounds each
        ///          component to the nearest integer and then adjusts one component to
        ///          ensure the `i + j + k = 0` invariant is maintained.
        /// @ref _ijkRound
        /// @param i The floating-point 'i' component.
        /// @param j The floating-point 'j' component.
        /// @param k The floating-point 'k' component.
        /// @return The nearest valid integer `ijk` coordinate.
        [[nodiscard]] static ijk from_cube_round(double i, double j, double k) noexcept;

        /// @brief Calculates the 2D Cartesian coordinates of the hexagon's center.
        /// @details Projects the IJK grid coordinate onto a 2D plane for visualization or
        ///          further geometric processing.
        /// @ref _ijkToHex2d
        /// @return A `vector2` containing the (x, y) coordinates.
        [[nodiscard]] vector2 center() const noexcept;

        /// @brief Adds two `ijk` coordinates component-wise.
        [[nodiscard]] ijk operator+(const ijk& item) const noexcept;

        /// @brief Subtracts two `ijk` coordinates component-wise.
        [[nodiscard]] ijk operator-(const ijk& item) const noexcept;

        /// @brief Scales an `ijk` coordinate by an integer factor.
        [[nodiscard]] ijk operator*(int factor) const noexcept;

        /// @brief Copy assignment operator.
        constexpr ijk& operator=(const ijk&) noexcept = default;

        /// @brief Move assignment operator.
        constexpr ijk& operator=(ijk&&) noexcept = default;

        /// @brief Equality comparison operator.
        /// @return `true` if all three components (`i`, `j`, `k`) are equal.
        constexpr bool operator==(const ijk&) const noexcept = default;

        /// @brief Checks if the IJK coordinates represent the grid origin {0, 0, 0}.
        [[nodiscard]] constexpr bool is_origin() const noexcept { return i == 0 && j == 0 && k == 0; }

        /// @brief In-place scales this IJK coordinate by an integer factor.
        /// @ref _ijkScale
        /// @param factor The integer scaling factor.
        void scale(int factor) noexcept;

        /// @brief In-place adds another `ijk` coordinate component-wise.
        void operator+=(const ijk& item) noexcept;

        /// @brief In-place subtracts another `ijk` coordinate component-wise.
        void operator-=(const ijk& item) noexcept;

        /// @brief In-place scales this IJK coordinate by an integer factor.
        void operator*=(int factor) noexcept { scale(factor); }

        /// @brief Adjusts IJK components to the nearest valid cube coordinate center.
        /// @details This function enforces the `i + j + k = 0` invariant. If the sum is
        ///          not zero, it rounds the components to find the closest point on the
        //           grid that satisfies the invariant.
        /// @ref _ijkNormalize
        void normalize() noexcept;

        /// @brief Converts a canonical unit IJK vector to its corresponding direction digit.
        /// @details This function is the inverse of `to_ijk(direction)`. It takes a unit
        ///          vector (like {0, 1, -1} for the J direction) and returns the
        ///          corresponding `direction_t` enum. It is essential for determining
        ///          the path of digits in grid traversal algorithms.
        /// @ref _unitIjkToDigit
        /// @return The `direction_t` corresponding to the unit vector, or
        ///         `direction_t::invalid` if the coordinate is not a unit vector.
        [[nodiscard]] direction_t to_digit() const noexcept;

        /// @brief Moves coordinates to the parent cell in a Class II (even resolution) grid.
        /// @details This performs an "aperture 7" grid traversal up the hierarchy.
        /// @ref _upAp7
        void up_ap7() noexcept;

        /// @brief Moves coordinates to the parent cell in a Class III (odd, rotated) grid.
        /// @details This performs an "aperture 7" grid traversal up the hierarchy for a
        ///          rotated grid.
        /// @ref _upAp7r
        void up_ap7r() noexcept;

        /// @brief Moves coordinates to the child cell center in a Class II grid.
        /// @details This is the inverse of `up_ap7`, used to descend the grid hierarchy.
        /// @ref _downAp7
        void down_ap7() noexcept;

        /// @brief Moves coordinates to the child cell center in a Class III grid.
        /// @details This is the inverse of `up_ap7r`, used to descend the grid hierarchy.
        /// @ref _downAp7r
        void down_ap7r() noexcept;

        /// @brief Returns a copy of this coordinate moved to a finer resolution grid.
        /// @param is_class_3 `true` if the target child resolution is Class III (odd).
        /// @return A new `ijk` coordinate at the child's scale.
        [[nodiscard]] ijk down_ap7(bool is_class_3) const noexcept;

        /// @brief Returns a copy of this coordinate moved to a coarser resolution grid.
        /// @param is_class_3 `true` if the current resolution is Class III (odd).
        /// @return A new `ijk` coordinate at the parent's scale.
        [[nodiscard]] ijk up_ap7_copy(bool is_class_3) const noexcept;

        /// @brief Moves this coordinate to a neighboring cell in a given direction.
        /// @ref _ijkNeighbor
        /// @param digit The direction to move.
        void to_neighbor(direction_t digit) noexcept;

        /// @brief Returns the neighboring cell's coordinates in a given direction.
        /// @param digit The direction to move.
        /// @return A new `ijk` coordinate representing the neighbor.
        [[nodiscard]] ijk neighbor(direction_t digit) const noexcept;

        /// @brief Rotates the coordinates 60 degrees counter-clockwise around the origin.
        /// @ref _ijkRotate60ccw
        void rotate_60ccw() noexcept;

        /// @brief Rotates the coordinates 60 degrees clockwise around the origin.
        /// @ref _ijkRotate60cw
        void rotate_60cw() noexcept;

        /// @brief Calculates the grid distance between two IJK coordinates.
        /// @details The hexagonal grid distance (number of cells to traverse) is half the
        ///          Manhattan distance of the cube coordinates.
        /// @ref ijkDistance
        /// @param b The other `ijk` coordinate.
        /// @return The grid distance.
        [[nodiscard]] int distance_to(const ijk& b) const noexcept;

        /// @brief Determines the leading non-zero direction digit for this coordinate.
        /// @details This is used to find the first step in the path from the base cell
        ///          to this coordinate, which is a key part of converting an IJK coordinate
        ///          back into a canonical H3 index.
        /// @ref _h3LeadingNonZeroDigit
        /// @param res The resolution of the grid system this coordinate exists in.
        /// @return The `direction_t` of the first non-center digit.
        [[nodiscard]] direction_t leading_digit(resolution_t res) const noexcept;

        /// @brief The 'k' component of the cube coordinate.
        value k;
    };

    /// @brief Creates a unit IJK vector for a given direction.
    /// @details This lookup table maps each of the 7 H3 directions to its corresponding
    ///          unit vector in the IJK cube coordinate system.
    /// @ref _neighbor
    /// @param direction The direction enum.
    /// @return The corresponding `ijk` unit vector.
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
    /// @details A helper function for projecting IJK coordinates onto a 2D plane.
    /// @ref _ijkToHex2d
    /// @param coord The `ijk` coordinate to convert.
    /// @return A 2D vector representing the coordinate's position.
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
