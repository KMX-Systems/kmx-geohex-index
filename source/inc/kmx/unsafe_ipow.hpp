/// @file unsafe_ipow.hpp
#pragma once
#ifndef PCH
    #include <array>
    #include <cstdint>
#endif

namespace kmx
{
    /// @brief It computes the value of base raised to the power exponent using bit-masking for fast exponentiation
    /// (See https://www.includehelp.com/algorithms/fast-exponentiation-using-bitmasking.aspx).
    /// The code is optimized by using an array of predetermined highest bits and by using loop-unrolling as
    /// an fallthrough switch instruction.
    /// @note No other overflow checks are made besides the case when exponent is greater than 62 - hence 'unsafe' notation.
    template <typename _Integer = std::int64_t>
    constexpr _Integer unsafe_ipow(_Integer base, std::uint8_t exponent) noexcept
    {
        _Integer result = 1;
        if (exponent < 63u) [[likely]]
        {
            static constexpr std::array<std::uint8_t, 64u> highest_bit {0u, 1u, 2u, 2u, 3u, 3u, 3u, 3u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u,
                                                                        5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u,
                                                                        6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
                                                                        6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 0u};
            switch (highest_bit[exponent])
            {
                case 6u:
                    if (exponent & 1u)
                        result *= base;
                    exponent >>= 1u;
                    base *= base;
                    [[fallthrough]];
                case 5u:
                    if (exponent & 1u)
                        result *= base;
                    exponent >>= 1u;
                    base *= base;
                    [[fallthrough]];
                case 4u:
                    if (exponent & 1u)
                        result *= base;
                    exponent >>= 1u;
                    base *= base;
                    [[fallthrough]];
                case 3u:
                    if (exponent & 1u)
                        result *= base;
                    exponent >>= 1u;
                    base *= base;
                    [[fallthrough]];
                case 2u:
                    if (exponent & 1u)
                        result *= base;
                    exponent >>= 1u;
                    base *= base;
                    [[fallthrough]];
                case 1u:
                    if (exponent & 1u)
                        result *= base;
                    break;
                default:
                    break;
            }
        }
        else // Handle special cases when exponent greater than 63 could cause overflow.
        {
            switch (base)
            {
                case _Integer(-1):
                    result -= (exponent & 1u) << 1u;
                    break;
                case 1:
                    break; // result is already set to 1.
                default:
                    result = 0; // 0 is returned in case of overflow.
                    break;
            }
        }

        return result;
    }
}
