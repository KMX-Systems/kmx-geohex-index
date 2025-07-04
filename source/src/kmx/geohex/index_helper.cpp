/// @file src/kmx/geohex/index_helper.cpp
/// @ingroup Internal
#include "kmx/geohex/index_helper.hpp"
#include "kmx/geohex/base.hpp"             // For error_t, resolution_t etc.
#include "kmx/geohex/icosahedron/face.hpp" // For ijk struct and functions
#include "kmx/geohex/traversal.hpp"
#include "kmx/gis/wgs84/coordinate.hpp" // For coordinate class

#if defined(_MSC_VER) && defined(_M_X64)
    #include <intrin.h>
    #pragma intrinsic(_BitScanReverse64)
#endif

namespace kmx::geohex
{
    // Private Static Validation Helper Implementations

    bool index_helper::has_invalid_digit_up_to_resolution(const value_t h, const resolution_t res) noexcept
    {
        // This is a bit-twiddling hack to check for the presence of 7 (0b111)
        static constexpr value_t mhi = 0x1249249249249249ULL; // 0b001001001...
        static constexpr value_t mlo = mhi >> 2;              // 0b000010010...

        const auto shift = digit_size * (max_resolution - static_cast<uint8_t>(res));
        const value_t digit_bits = (h << (64 - base_cell_pos)) >> (64 - base_cell_pos);
        const value_t relevant_digits = digit_bits >> shift;

        return (relevant_digits & mhi & (~relevant_digits - mlo)) != 0;
    }

    uint32_t index_helper::first_one_index(const value_t h) noexcept
    {
#if defined(__GNUC__) || defined(__clang__)
        return 63u - __builtin_clzll(h);
#elif defined(_MSC_VER) && defined(_M_X64)
        unsigned long index;
        if (_BitScanReverse64(&index, h))
            return static_cast<uint32_t>(index);
        return 0;
#else
        for (int i = 44; i >= 0; --i)
            if ((h >> i) & 1)
                return i;
        return 0;
#endif
    }

    bool index_helper::has_deleted_subsequence(const value_t h, const cell::base::id_t base_cell) noexcept
    {
        if (cell::pentagon::check(base_cell))
        {
            const value_t k_axis_digit_mask = digit_mask << (digit_size * (max_resolution - 1));
            if ((h & k_axis_digit_mask) == k_axis_digit_mask)
                return true;
        }
        return false;
    }

    // index_helper Member Function Implementations

    bool index_helper::is_valid() const noexcept
    {
        // Common checks for all modes: resolution and base cell must be in a valid range.
        const auto res = resolution();
        if (static_cast<uint8_t>(res) > max_resolution)
            return false;

        const auto bc = base_cell();
        if (bc >= cell::base::count)
            return false;

        // Dispatch to mode-specific validation logic.
        switch (mode())
        {
            case index_mode_t::cell:
            {
                // Cell-Specific Validation
                // A cell index must have its mode bits set to 1.
                if ((value_ >> mode_pos) != +index_mode_t::cell)
                    return false;

                // Check for invalid digits (7) in the resolution part of the index.
                if (has_invalid_digit_up_to_resolution(value_, res))
                    return false;

                // Check for pentagon-specific invalid sequences (e.g., k-axis).
                if (has_deleted_subsequence(value_, bc))
                    return false;

                return true;
            }
            case index_mode_t::edge_unidirectional:
            {
                // Unidirectional Edge-Specific Validation
                // An edge index must have its mode bits set to 2.
                if ((value_ >> mode_pos) != +index_mode_t::edge_unidirectional)
                    return false;

                // The edge direction, stored in the reserved bits, must be valid (1-6).
                const auto dir = edge_direction();
                if ((dir < direction_t::k_axes) || (dir > direction_t::ij_axes))
                    return false;

                // Edges also must not have invalid digits.
                if (has_invalid_digit_up_to_resolution(value_, res))
                    return false;

                return true;
            }
            case index_mode_t::vertex:
            {
                // Vertex-Specific Validation
                // A vertex index must have its mode bits set to 5.
                if ((value_ >> mode_pos) != +index_mode_t::vertex)
                    return false;

                // The vertex number, stored in reserved bits, must be valid (0-5).
                if (vertex_number() > 5u)
                    return false;

                // Vertices also must not have invalid digits.
                if (has_invalid_digit_up_to_resolution(value_, res))
                    return false;

                return true;
            }
            // Other modes like 'edge_bidirectional' could be added here.
            case index_mode_t::invalid:
            default:
                // Any other mode is, by definition, invalid.
                return false;
        }
    }

    bool index_helper::is_pentagon() const noexcept
    {
        return cell::pentagon::check(base_cell()) && (leading_non_zero_digit() == direction_t::center);
    }

    index_mode_t index_helper::mode() const noexcept
    {
        return static_cast<index_mode_t>((value_ >> mode_pos) & mode_mask);
    }

    void index_helper::set_mode(const index_mode_t item) noexcept
    {
        value_ = (value_ & ~(mode_mask << mode_pos)) | (static_cast<value_t>(item) & mode_mask) << mode_pos;
    }

    resolution_t index_helper::resolution() const noexcept
    {
        return static_cast<resolution_t>((value_ >> resolution_pos) & resolution_mask);
    }

    void index_helper::set_resolution(const resolution_t item) noexcept
    {
        value_ = (value_ & ~(resolution_mask << resolution_pos)) | (static_cast<value_t>(item) & resolution_mask) << resolution_pos;
    }

    cell::base::id_t index_helper::base_cell() const noexcept
    {
        return static_cast<cell::base::id_t>((value_ >> base_cell_pos) & base_cell_mask);
    }

    void index_helper::set_base_cell(const cell::base::id_t item) noexcept
    {
        value_ = (value_ & ~(base_cell_mask << base_cell_pos)) | (static_cast<value_t>(item) & base_cell_mask) << base_cell_pos;
    }

    std::uint8_t index_helper::shift_from(const digit_index index) noexcept
    {
        return (max_resolution - 1u - index) * digit_size;
    }

    digit_t index_helper::digit(const digit_index index) const noexcept
    {
        if (index >= digit_count())
            return 0u;

        return (value_ >> shift_from(index)) & digit_mask;
    }

    void index_helper::set_digit(const digit_index index, const digit_t item) noexcept
    {
        if (index < digit_count())
        {
            const auto shift = shift_from(index);
            value_ = (value_ & ~(digit_mask << shift)) | ((static_cast<value_t>(item) & digit_mask) << shift);
        }
    }

    bool index_helper::set_digits_to_zero(const digit_index start, const digit_index end) noexcept
    {
        if ((start > end) || (end >= digit_count()))
            return false;

        for (digit_index i = start; i <= end; ++i)
            set_digit(i, 0u);

        return true;
    }

    direction_t index_helper::leading_non_zero_digit() const noexcept
    {
        const auto res = static_cast<uint8_t>(resolution());
        for (digit_index i {}; i < res; ++i)
        {
            const auto d = digit(i);
            if (d != 0)
                return static_cast<direction_t>(d);
        }

        return direction_t::center;
    }

    [[nodiscard]] direction_t index_helper::edge_direction() const noexcept
    {
        return static_cast<direction_t>((value_ >> mode_dependent_pos) & mode_dependent_mask);
    }

    void index_helper::set_edge_direction(const direction_t direction) noexcept
    {
        // Create an inverted mask to clear the target bits without affecting others.
        constexpr value_t inverted_mask = ~(mode_dependent_mask << mode_dependent_pos);
        value_ &= inverted_mask;

        // Mask the input, shift it into position, and OR it into the value.
        value_ |= (static_cast<value_t>(direction) & mode_dependent_mask) << mode_dependent_pos;
    }

    [[nodiscard]] std::uint8_t index_helper::vertex_number() const noexcept
    {
        return static_cast<std::uint8_t>((value_ >> mode_dependent_pos) & mode_dependent_mask);
    }

    void index_helper::set_vertex_number(const vertex_no_t vertex_no) noexcept
    {
        // 1. Create the inverted mask
        // This mask will have zeros at the position of the mode-dependent bits
        // and ones everywhere else.
        // `~0ULL` creates an all-ones 64-bit integer.
        // `~(mode_dependent_mask << mode_dependent_pos)` creates the "hole".
        constexpr value_t inverted_mask = ~(mode_dependent_mask << mode_dependent_pos);

        // 2. Clear the target bits
        // Applying the inverted mask with a bitwise AND operation will zero out the
        // three mode-dependent bits while leaving all other bits untouched.
        value_ &= inverted_mask;

        // 3. Set the new bits
        // a. Take the integer `vertex_no`, cast it to our value_t, and mask it to
        //    ensure it doesn't exceed the 3-bit range (0-7). This is a safety measure.
        // b. Shift the masked value to the correct position.
        // c. Use a bitwise OR operation to merge the new bits into the cleared value.
        value_ |= (static_cast<value_t>(vertex_no) & mode_dependent_mask) << mode_dependent_pos;
    }

    // Detail-level Free Function Implementations

    error_t to_wgs(const raw_index_t idx, gis::wgs84::coordinate& coord) noexcept
    {
        const index_helper helper {idx};
        if (!helper.is_valid())
            return error_t::cell_invalid;

        icosahedron::face::ijk fijk;
        const error_t err = icosahedron::face::from_index(idx, fijk);
        if (err != error_t::none)
            return err;

        return icosahedron::face::to_wgs(fijk, helper.resolution(), coord);
    }

    raw_index_t from_wgs(const gis::wgs84::coordinate& coord, const resolution_t res) noexcept
    {
        // Step 1: Convert the public geographic coordinate into the internal FaceIJK
        // representation. This is the most complex part of the algorithm, as it
        // involves projecting the coordinate onto the icosahedron and finding the
        // best-fit face and IJK grid cell.
        icosahedron::face::ijk fijk;
        const error_t err_geo_to_fijk = icosahedron::face::from_wgs(coord, res, fijk);

        if (err_geo_to_fijk != error_t::none)
            // The conversion from geographic coordinates failed, likely due to an
            // invalid input (e.g., NaN) or an internal projection error.
            return 0u; // Return invalid index

        // Step 2: Convert the internal FaceIJK representation into a final, canonical
        // index. This process determines the base cell and the sequence of digits
        // that define the path from the base cell to the target cell.
        index result_index;
        const error_t err_fijk_to_h3 = icosahedron::face::to_index(fijk, res, result_index);

        if (err_fijk_to_h3 != error_t::none)
            // The conversion from the internal representation failed. This could happen
            // if the FaceIJK coordinates were somehow invalid after the first step.
            return 0u; // Return invalid index

        // If both steps succeeded, return the raw 64-bit value of the resulting index.
        return result_index.value();
    }

    std::uint64_t children_count(const raw_index_t parent_idx, const resolution_t child_res) noexcept
    {
        const index_helper helper {parent_idx};
        if (!helper.is_valid() || (child_res <= helper.resolution()))
            return {};

        const auto res_diff = +child_res - +helper.resolution();

        // The maximum possible res_diff is 15 (from res 0 to 15).
        // 7^15 is a very large number, so std::uint64_t is the correct type.
        const std::uint64_t num_children = unsafe_ipow<std::uint64_t>(7u, res_diff);

        // This is the final piece: you must account for pentagons, which have fewer children.
        // The cell::children_count function does this, but this free function was missing it.
        return helper.is_pentagon() ?
                   // This is the formula for pentagon children count.
                   1u + 5u * (num_children - 1u) / 6u :
                   num_children;
    }

    [[nodiscard]] error_t get_children(const raw_index_t parent_idx, const resolution_t child_res,
                                       std::span<raw_index_t>& out_children) noexcept
    {
        const index parent_index {parent_idx};
        const resolution_t parent_res = parent_index.resolution();

        // 1. Validate inputs.
        if (!parent_index.is_valid() || (child_res <= parent_res))
        {
            out_children = out_children.subspan(0u, 0u);
            return error_t::domain;
        }

        // 2. Validate the output buffer size.
        const auto required_size = children_count(parent_idx, child_res);
        if (out_children.size() < required_size)
            return error_t::buffer_too_small;

        // 3. Core algorithm implementation.
        const bool is_parent_pentagon = cell::pentagon::check(parent_index.base_cell());
        const std::int32_t res_diff = +child_res - +parent_res;
        const std::int32_t k_radius = unsafe_ipow<std::int32_t>(7, res_diff) / 2;

        std::size_t children_written {};

        // Iterate through all possible local IJK coordinates within the child group's k-ring.
        for (std::int32_t i = -k_radius; i <= k_radius; ++i)
        {
            for (std::int32_t j = -k_radius; j <= k_radius; ++j)
            {
                const std::int32_t k = -i - j;
                if (std::abs(k) > k_radius)
                    continue;

                coordinate::ijk local_ijk {i, j, k};

                // For pentagons, the conceptual child grid is rotated.
                if (is_parent_pentagon)
                    local_ijk.rotate_60ccw();

                // Check if this candidate is a true child by ascending the hierarchy.
                coordinate::ijk parent_check_ijk = local_ijk;
                for (std::int32_t r {}; r < res_diff; ++r)
                {
                    const bool is_class_3_res = is_class_3(static_cast<resolution_t>(+child_res - r));
                    parent_check_ijk = parent_check_ijk.up_ap7_copy(is_class_3_res);
                }

                // If the ascended IJK is not the center, it's not a direct child.
                if (parent_check_ijk.is_origin())
                {
                    // Convert the valid local IJK coordinate back into a global H3 index.
                    index child_index {};
                    const error_t err = local_ijk_to_index(parent_index, local_ijk, child_index);

                    if (err == error_t::none)
                    {
                        if (children_written < required_size)
                            out_children[children_written++] = child_index.value();
                        else
                            // This case should not be reached if required_size is correct.
                            return error_t::failed;
                    }
                    else
                    {
                        out_children = out_children.subspan(0u, 0u);
                        return err;
                    }
                }
            }
        }

        // 4. Final validation and span resizing.
        if (children_written != required_size)
            return error_t::failed; // Indicates a logic error in the algorithm.

        out_children = out_children.subspan(0u, children_written);
        return error_t::none;
    }

    raw_index_t get_parent(const raw_index_t child_idx, const resolution_t parent_res) noexcept
    {
        index_helper helper {child_idx};
        if (!helper.is_valid() || parent_res >= helper.resolution())
            return 0u;

        helper.set_resolution(parent_res);
        for (digit_index i = static_cast<digit_index>(parent_res); i < index_helper::digit_count(); ++i)
            helper.set_digit(i, 7u); // Set to invalid digit

        return helper.value();
    }
} // namespace kmx::geohex::detail
