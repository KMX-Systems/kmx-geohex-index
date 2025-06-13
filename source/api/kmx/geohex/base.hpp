/// @file api/kmx/geohex/base.hpp
/// @ingroup API
/// @brief Core enumerations, type aliases, and fundamental utilities for the GeoHex library.
#pragma once
#ifndef PCH
    #include <cstdint>
    #include <functional>
#endif

namespace kmx
{
    /// @brief Provides a convenient way to get the underlying integer value of a strongly-typed enum.
    /// @tparam T An enum type.
    /// @param e The enum value.
    /// @return The underlying integral value of the enum.
    template <typename T>
        requires std::is_enum_v<T>
    constexpr auto operator+(const T e) noexcept
    {
        return static_cast<std::underlying_type_t<T>>(e);
    }

    /// @brief Combines a new hash value with an existing seed.
    /// @details A standard technique to combine multiple hash values into one,
    ///          inspired by `boost::hash_combine`.
    /// @param[in,out] seed The seed to combine with.
    /// @param v The value to hash and combine.
    template <class T>
    void hash_combine(std::size_t& seed, const T& v) noexcept
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9u + (seed << 6u) + (seed >> 2u);
    }
}

namespace kmx::geohex
{
    using raw_index_t = std::uint64_t;
    using digit_index = std::uint8_t;
    using digit_t = std::uint8_t;

    /// @brief Defines the different H3 index modes (cell, edge, vertex).
    enum class index_mode_t : std::uint8_t
    {
        invalid,
        cell,
        edge_unidirectional,
        edge_bidirectional,
        vertex
    };

    /// @brief Defines the 16 H3 grid resolutions, from 0 (coarsest) to 15 (finest).
    enum class resolution_t : std::uint8_t
    {
        r0,
        r1,
        r2,
        r3,
        r4,
        r5,
        r6,
        r7,
        r8,
        r9,
        r10,
        r11,
        r12,
        r13,
        r14,
        r15
    };

    /// @brief The total number of H3 resolutions available.
    constexpr auto resolution_count = +resolution_t::r15 + 1u;

    /// @brief Determines if a resolution is Class III (odd).
    /// @details Class III resolutions have a different grid orientation than Class II (even) resolutions.
    /// @param res The resolution to check.
    /// @return True if the resolution is odd (Class III), false otherwise.
    constexpr bool is_class_3(const resolution_t res) noexcept
    {
        return (+res % 2u) != 0u;
    }

    /// @brief Defines the 7 directions in the H3 grid system, including the center.
    enum class direction_t : std::uint8_t
    {
        center,
        k_axes,
        j_axes,
        jk_axes,
        i_axes,
        ik_axes,
        ij_axes,
        invalid
    };

    /// @brief The number of valid H3 directions (used for array sizing).
    constexpr auto direction_count = +direction_t::invalid;

    /// @brief Defines error codes returned by library functions.
    enum class error_t : std::uint8_t
    {
        none = 0,
        failed,
        domain,
        latlng_domain,
        res_domain,
        cell_invalid,
        dir_edge_invalid,
        undir_edge_invalid,
        vertex_invalid,
        pentagon,
        duplicate_input,
        not_neighbors,
        res_mismatch,
        memory_alloc,
        memory_bounds,
        option_invalid,
        not_supported,
        buffer_too_small
    };
}

namespace kmx::geohex::cell::base
{
    using id_t = std::uint8_t;

    /// @ref res0CellCount
    constexpr id_t count = 122u;
}
