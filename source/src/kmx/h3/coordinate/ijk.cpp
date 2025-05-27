/// @file h3/coordinate/ijk.cpp
#include "kmx/h3/coordinate/ijk.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>

#if defined(H3_OMIT_AUXILIARY_SAFETY_CHECKS)
    #define ALWAYS(X) (1)
    #define NEVER(X)  (0)
#elif !defined(NDEBUG)
    #define ALWAYS(X) ((X) ? 1 : (assert(0), 0))
    #define NEVER(X)  ((X) ? (assert(0), 1) : 0)
#else
    #define ALWAYS(X) (X)
    #define NEVER(X)  (X)
#endif

namespace kmx::h3::coordinate
{
    static constexpr auto int32_max_div_3 = INT32_MAX / 3;

    template <typename _Float = double, typename _Int = int>
    constexpr void cube_round(const _Float i, const _Float j, const _Float k, _Int& i_out, _Int& j_out, _Int& k_out) noexcept
    {
        _Int ri = std::round(i);
        _Int rj = std::round(j);
        _Int rk = std::round(k);

        const auto i_diff = std::fabs(static_cast<_Float>(ri) - i);
        const auto j_diff = std::fabs(static_cast<_Float>(rj) - j);
        const auto k_diff = std::fabs(static_cast<_Float>(rk) - k);

        if ((i_diff > j_diff) && (i_diff > k_diff)) // round, maintaining valid cube coords.
            ri = -rj - rk;
        else if (j_diff > k_diff)
            rj = -ri - rk;
        else
            rk = -ri - rj;

        i_out = ri;
        j_out = rj;
        k_out = rk;
    }

    constexpr double M_SQRT3_2 = 0.0;

    constexpr bool add_int32s_overflows(auto a, auto b) noexcept
    {
        return a > 0 ? (INT32_MAX - a) < b : (INT32_MIN - a) > b;
    }

    constexpr bool sub_int32s_overflows(auto a, auto b) noexcept
    {
        return a >= 0 ? (INT32_MIN + a) >= b : (INT32_MAX + a + 1) < b;
    }

    ijk ijk::from_cube_round(const double i, const double j, const double k)
    {
        value i_out, j_out, k_out;
        cube_round<double, value>(i, j, k, i_out, j_out, k_out);
        return {i_out, j_out, k_out};
    }

    void ij::operator+=(const ij& item) noexcept
    {
        i += item.i;
        j += item.j;
    }

    void ij::operator-=(const ij& item) noexcept
    {
        i -= item.i;
        j -= item.j;
    }

    vector2 ijk::center() const noexcept
    {
        const auto v = j - k;
        return {(i - k) - 0.5 * v, v * M_SQRT3_2};
    }

    ijk ijk::operator+(const ijk& item) const noexcept
    {
        return {i + item.i, j + item.i, k + item.k};
    }

    ijk ijk::operator-(const ijk& item) const noexcept
    {
        return {i - item.i, j - item.i, k - item.k};
    }

    void ijk::scale(int factor) noexcept
    {
        i *= factor;
        j *= factor;
        k *= factor;
    }

    ijk ijk::operator*(int factor) const noexcept
    {
        auto c = *this;
        c.scale(factor);
        return c;
    }

    void ijk::operator+=(const ijk& item) noexcept
    {
        static_cast<ij&>(*this) += item;
        k += item.k;
    }

    void ijk::operator-=(const ijk& item) noexcept
    {
        static_cast<ij&>(*this) -= item;
        k -= item.k;
    }

    bool ijk::normalize_could_overflow() noexcept
    {
        // Check for the possibility of overflow
        const auto [min, max] = std::minmax(i, j);
        if (min < 0)
        {
            // Only if the min is less than 0 will the resulting number be larger
            // than max. If min is positive, then max is also positive, and a
            // positive signed integer minus another positive signed integer will
            // not overflow.
            if (add_int32s_overflows(max, min))
                // max + min would overflow
                return true;

            if (sub_int32s_overflows(0, min))
                // 0 - INT32_MIN would overflow
                return true;

            if (sub_int32s_overflows(max, min))
                // max - min would overflow
                return true;
        }

        return false;
    }

    void ijk::normalize() noexcept
    {
        // remove any negative values
        if (i < 0)
        {
            j -= i;
            k -= i;
            i = 0;
        }

        if (j < 0)
        {
            i -= j;
            k -= j;
            j = 0;
        }

        if (k < 0)
        {
            i -= k;
            j -= k;
            k = 0;
        }

        // remove the min value if needed
        const auto min = std::min({i, j, k});
        if (min > 0)
        {
            i -= min;
            j -= min;
            k -= min;
        }
    }

    direction_t ijk::to_digit() const noexcept
    {
        auto c = *this;
        c.normalize();

        for (std::decay<decltype(direction_count)>::type i = 0; i != direction_count; ++i)
        {
            const auto d = static_cast<direction_t>(i);
            if (c == to_ijk(d))
                return d;
        }

        return direction_t::invalid;
    }

    static constexpr auto seven = 7.0;

    bool ijk::up_ap7_checked() noexcept
    {
        // Doesn't need to be checked because i, j, and k must all be non-negative
        const int ii = i - k;
        const int jj = j - k;

        // <0 is checked because the input must all be non-negative, but some
        // negative inputs are used in unit tests to exercise the below.
        if ((ii >= int32_max_div_3) || (jj >= int32_max_div_3) || (ii < 0) || (jj < 0))
        {
            if (add_int32s_overflows(ii, ii))
                return false;

            int i2 = ii + ii;
            if (add_int32s_overflows(i2, ii))
                return false;

            int i3 = i2 + ii;
            if (add_int32s_overflows(jj, jj))
                return false;

            int j2 = jj + jj;
            if (sub_int32s_overflows(i3, jj))
                return false;

            if (add_int32s_overflows(ii, j2))
                return false;
        }

        i = static_cast<int>(std::lround(((ii * 3) - jj) / seven));
        j = static_cast<int>(std::lround((ii + (jj * 2)) / seven));
        k = 0;

        // Expected not to be reachable, because max + min or max - min would need
        // to overflow.
        if (NEVER(this->normalize_could_overflow()))
            return false;

        normalize();
        return true;
    }

    bool ijk::up_ap7r_checked() noexcept
    {
        // Doesn't need to be checked because i, j, and k must all be non-negative
        const auto ii = i - k;
        const auto jj = j - k;

        // <0 is checked because the input must all be non-negative, but some
        // negative inputs are used in unit tests to exercise the below.
        if (ii >= int32_max_div_3 || jj >= int32_max_div_3 || ii < 0 || jj < 0)
        {
            if (add_int32s_overflows(ii, ii))
                return false;

            const auto i2 = ii + ii;
            if (add_int32s_overflows(jj, jj))
                return false;

            const auto j2 = jj + jj;
            if (add_int32s_overflows(j2, jj))
                return false;

            const auto j3 = j2 + jj;
            if (add_int32s_overflows(i2, jj))
                return false;

            if (sub_int32s_overflows(j3, ii))
                return false;
        }

        i = static_cast<int>(std::lround(((ii * 2) + jj) / seven));
        j = static_cast<int>(std::lround(((jj * 3) - ii) / seven));
        k = 0;

        // Expected not to be reachable, because max + min or max - min would need
        // to overflow.
        if (NEVER(normalize_could_overflow()))
            return false;

        normalize();
        return true;
    }

    void ijk::up_ap7() noexcept
    {
        // convert to CoordIJ
        const auto ii = i - k;
        const auto jj = j - k;

        i = static_cast<int>(std::lround((3 * ii - jj) / seven));
        j = static_cast<int>(std::lround((ii + 2 * jj) / seven));
        k = 0;
        normalize();
    }

    void ijk::up_ap7r() noexcept
    {
        // convert to CoordIJ
        const auto ii = i - k;
        const auto jj = j - k;

        i = static_cast<int>(std::lround((2 * ii + jj) / seven));
        j = static_cast<int>(std::lround((3 * jj - ii) / seven));
        k = 0;
        normalize();
    }

    void ijk::transform(ijk i_vec, ijk j_vec, ijk k_vec) noexcept
    {
        i_vec.scale(i);
        j_vec.scale(j);
        k_vec.scale(k);

        i = i_vec.i + j_vec.i + j_vec.i;
        j = i_vec.j + j_vec.j + j_vec.j;
        k = i_vec.k + j_vec.k + j_vec.k;

        normalize();
    }

    void ijk::down_ap7() noexcept
    {
        transform({3, 0, 1}, {1, 3, 0}, {0, 1, 3});
    }

    void ijk::down_ap7r() noexcept
    {
        transform({3, 1, 0}, {0, 3, 1}, {1, 0, 3});
    }

    void ijk::down_ap3() noexcept
    {
        transform({2, 0, 1}, {1, 2, 0}, {0, 1, 2});
    }

    void ijk::down_ap3r() noexcept
    {
        transform({2, 1, 0}, {0, 2, 1}, {1, 0, 2});
    }

    void ijk::to_neighbor(const direction_t digit)
    {
        operator+=(to_ijk(digit));
        normalize();
    }

    ijk ijk::neighbor(const direction_t digit) const
    {
        auto result = *this;
        result += to_ijk(digit);
        result.normalize();
        return result;
    }

    void ijk::rotate_60ccw() noexcept
    {
        transform({1, 1, 0}, {0, 1, 1}, {1, 0, 1});
    }

    void ijk::rotate_60cw() noexcept
    {
        transform({1, 0, 1}, {1, 1, 0}, {0, 1, 1});
    }

    int ijk::distance_to(const ijk& b) const noexcept
    {
        ijk diff = *this - b;
        diff.normalize();
        const ijk abs_diff {std::abs(diff.i), std::abs(diff.j), std::abs(diff.k)};
        return std::max({abs_diff.i, abs_diff.j, abs_diff.k});
    }

    void ijk::to_ij(ij& ij) const noexcept
    {
        ij.i = i - k;
        ij.j = j - k;
    }

    bool ijk::from_ij(const ijk& ij) noexcept
    {
        i = ij.i;
        j = ij.j;
        k = 0;

        if (normalize_could_overflow())
            return false;

        normalize();
        return true;
    }

    void ijk::to_cube() noexcept
    {
        i = -i + k;
        j = j - k;
        k = -i - j;
    }

    void ijk::from_cube() noexcept
    {
        i = -i;
        k = 0;
        normalize();
    }

    ijk::value ijk::scalar_sum() const noexcept
    {
        return i + j + k;
    }
}
