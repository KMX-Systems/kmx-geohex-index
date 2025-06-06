/// @file geohex/coordinate/ij.hpp
#pragma once
#ifndef PCH
    #include <cstdint>
#endif

namespace kmx::geohex::coordinate
{
    struct ij
    {
        using value = std::int32_t;

        constexpr ij() noexcept = default;
        constexpr ij(const ij&) noexcept = default;
        constexpr ij(ij&&) noexcept = default;
        constexpr ij(const value i, const value j) noexcept: i(i), j(j) {}
        constexpr ij(const value data[3u]) noexcept: i(data[0u]), j(data[1u]) {}

        constexpr ij& operator=(const ij&) noexcept = default;
        constexpr ij& operator=(ij&&) noexcept = default;

        constexpr bool operator==(const ij&) const noexcept = default;
        constexpr bool operator!=(const ij&) const noexcept = default;

        void operator+=(const ij& item) noexcept;
        void operator-=(const ij& item) noexcept;

        value i, j;
    };
}
