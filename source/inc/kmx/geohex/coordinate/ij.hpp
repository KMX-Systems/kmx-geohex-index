/// @file inc/kmx/geohex/coordinate/ij.hpp
/// @ingroup Internal
/// @brief Defines the 2D (IJ) hexagonal axial coordinate class.
#pragma once
#ifndef PCH
    #include <cstdint>
#endif

namespace kmx::geohex::coordinate
{
    /// @brief Represents a 2D hexagonal grid coordinate using an axial system.
    /// @details In an axial coordinate system, two axes (i and j) are used to define a
    ///          position on the hexagonal grid. This struct is primarily used as a
    ///          building block for the `ijk` class, which adds a third dependent
    ///          coordinate (`k`) to form the "cube coordinate" system where `i+j+k=0`.
    struct ij
    {
        /// @brief The underlying integer type for coordinate components.
        using value = std::int32_t;

        /// @brief Default constructor. Initializes components to zero.
        constexpr ij() noexcept = default;

        /// @brief Copy constructor.
        constexpr ij(const ij&) noexcept = default;

        /// @brief Move constructor.
        constexpr ij(ij&&) noexcept = default;

        /// @brief Constructs an `ij` coordinate from individual `i` and `j` components.
        /// @param i The 'i' component of the axial coordinate.
        /// @param j The 'j' component of the axial coordinate.
        constexpr ij(const value i, const value j) noexcept: i(i), j(j) {}

        /// @brief Constructs an `ij` coordinate from a C-style array.
        /// @param data A C-style array where `data[0]` is the 'i' component and
        ///             `data[1]` is the 'j' component.
        constexpr ij(const value data[2u]) noexcept: i(data[0u]), j(data[1u]) {}

        /// @brief Copy assignment operator.
        constexpr ij& operator=(const ij&) noexcept = default;

        /// @brief Move assignment operator.
        constexpr ij& operator=(ij&&) noexcept = default;

        /// @brief Equality comparison operator.
        /// @return `true` if both `i` and `j` components are equal, `false` otherwise.
        constexpr bool operator==(const ij&) const noexcept = default;

        /// @brief Inequality comparison operator.
        /// @return `true` if either the `i` or `j` components are not equal, `false` otherwise.
        constexpr bool operator!=(const ij&) const noexcept = default;

        /// @brief In-place addition operator. Adds the components of another `ij` coordinate.
        /// @param item The `ij` coordinate to add.
        void operator+=(const ij& item) noexcept;

        /// @brief In-place subtraction operator. Subtracts the components of another `ij` coordinate.
        /// @param item The `ij` coordinate to subtract.
        void operator-=(const ij& item) noexcept;

        /// @brief The 'i' component of the axial coordinate.
        value i {};

        /// @brief The 'j' component of the axial coordinate.
        value j {};
    };
}
