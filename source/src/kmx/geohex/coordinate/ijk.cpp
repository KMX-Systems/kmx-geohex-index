/// @file src/kmx/geohex/coordinate/ijk.cpp
/// @ingroup Internal
#include "kmx/geohex/coordinate/ijk.hpp"
#include "kmx/geohex/utils.hpp"
#include <algorithm>
#include <cmath>

namespace kmx::geohex::coordinate
{
    ijk ijk::from_cube_round(const double i, const double j, const double k) noexcept
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
        return to_vec2<double>(*this);
    }

    ijk ijk::operator+(const ijk& item) const noexcept
    {
        return {i + item.i, j + item.j, k + item.k};
    }

    ijk ijk::operator-(const ijk& item) const noexcept
    {
        return {i - item.i, j - item.j, k - item.k};
    }

    ijk ijk::operator*(const int factor) const noexcept
    {
        return {i * factor, j * factor, k * factor};
    }

    void ijk::scale(const int factor) noexcept
    {
        i *= factor;
        j *= factor;
        k *= factor;
    }

    void ijk::operator+=(const ijk& item) noexcept
    {
        i += item.i;
        j += item.j;
        k += item.k;
    }

    void ijk::operator-=(const ijk& item) noexcept
    {
        i -= item.i;
        j -= item.j;
        k -= item.k;
    }

    void ijk::normalize() noexcept
    {
        // The sum of the components must be 0 for a valid cube coordinate.
        // This process rounds the values to the nearest valid integer coordinate.
        if (i + j + k != 0)
        {
            value i_out, j_out, k_out;
            cube_round<value, value>(i, j, k, i_out, j_out, k_out);
            i = i_out;
            j = j_out;
            k = k_out;
        }
    }

    direction_t ijk::to_digit() const noexcept
    {
        // This is only valid for IJK vectors that are unit vectors from the origin.
        // It's a key part of the grid ascent/descent algorithms.
        ijk c = *this;
        c.normalize();

        for (uint8_t digit_val = 0u; digit_val < direction_count; ++digit_val)
            if (c == to_ijk(static_cast<direction_t>(digit_val)))
                return static_cast<direction_t>(digit_val);

        return direction_t::invalid;
    }

    void ijk::up_ap7() noexcept
    {
        // This is the fast algebraic simplification of the Class II parent-finding matrix transform.
        const double i_prime = static_cast<double>(i);
        const double j_prime = static_cast<double>(j);
        const double k_prime = static_cast<double>(k);
        i = static_cast<value>(std::round((3.0 * i_prime - 1.0 * j_prime - 1.0 * k_prime) / 7.0));
        j = static_cast<value>(std::round((-1.0 * i_prime + 3.0 * j_prime - 1.0 * k_prime) / 7.0));
        k = static_cast<value>(std::round((-1.0 * i_prime - 1.0 * j_prime + 3.0 * k_prime) / 7.0));
        normalize();
    }

    void ijk::up_ap7r() noexcept
    {
        // This is the fast algebraic simplification of the Class III parent-finding matrix transform.
        const double i_prime = static_cast<double>(i);
        const double j_prime = static_cast<double>(j);
        const double k_prime = static_cast<double>(k);
        i = static_cast<value>(std::round((2.0 * i_prime + 1.0 * j_prime - 1.0 * k_prime) / 7.0));
        j = static_cast<value>(std::round((-1.0 * i_prime + 2.0 * j_prime + 1.0 * k_prime) / 7.0));
        k = static_cast<value>(std::round((1.0 * i_prime - 1.0 * j_prime + 2.0 * k_prime) / 7.0));
        normalize();
    }

    void ijk::down_ap7() noexcept
    {
        // Optimized replacement for the transform method, using the inverse matrix of up_ap7.
        const value i_prime = i;
        const value j_prime = j;
        i = 2 * i_prime - j_prime - k;
        j = -i_prime + 2 * j_prime - k;
        k = -i_prime - j_prime + 2 * k;
    }

    void ijk::down_ap7r() noexcept
    {
        // Optimized replacement for the transform method, using the inverse matrix of up_ap7r.
        const value i_prime = i;
        const value j_prime = j;
        i = 2 * i_prime + j_prime - k;
        j = -i_prime + 2 * j_prime + k;
        k = i_prime - j_prime + 2 * k;
    }

    ijk ijk::down_ap7(const bool is_class_3) const noexcept
    {
        ijk next_ijk = *this;
        if (is_class_3)
            next_ijk.down_ap7r();
        else
            next_ijk.down_ap7();

        return next_ijk;
    }

    ijk ijk::up_ap7_copy(const bool is_class_3) const noexcept
    {
        ijk next_ijk = *this;
        if (is_class_3)
            next_ijk.up_ap7r();
        else
            next_ijk.up_ap7();

        return next_ijk;
    }

    void ijk::to_neighbor(const direction_t digit) noexcept
    {
        *this += to_ijk(digit);
    }

    ijk ijk::neighbor(const direction_t digit) const noexcept
    {
        return *this + to_ijk(digit);
    }

    void ijk::rotate_60ccw() noexcept
    {
        // A 60-degree counter-clockwise rotation in cube coordinates is (i, j, k) -> (-j, -k, -i).
        const value temp_i = i;
        i = -j;
        j = -k;
        k = -temp_i;
    }

    void ijk::rotate_60cw() noexcept
    {
        // A 60-degree clockwise rotation in cube coordinates is (i, j, k) -> (-k, -i, -j).
        const value temp_i = i;
        i = -k;
        k = -j;
        j = -temp_i;
    }

    int ijk::distance_to(const ijk& b) const noexcept
    {
        const ijk diff = *this - b;
        // The distance in a hexagonal grid is the max of the absolute differences
        // of the cube coordinates.
        const ijk abs_diff {std::abs(diff.i), std::abs(diff.j), std::abs(diff.k)};
        return std::max({abs_diff.i, abs_diff.j, abs_diff.k});
    }

    direction_t ijk::leading_digit(const resolution_t res) const noexcept
    {
        ijk current_ijk = *this;
        for (resolution_t r = res; +r > 0u; r = static_cast<resolution_t>(+r - 1u))
        {
            const ijk parent_ijk = current_ijk.up_ap7_copy(is_class_3(r));
            const ijk child_at_parent_res = parent_ijk.down_ap7(is_class_3(r));
            const ijk diff = current_ijk - child_at_parent_res;

            const direction_t digit = diff.to_digit();
            if (digit != direction_t::center)
                return digit;

            current_ijk = parent_ijk;
        }

        return direction_t::center;
    }

} // namespace kmx::geohex::coordinate
