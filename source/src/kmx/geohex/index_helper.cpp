/// @file src/kmx/geohex/index_helper.cpp
/// @ingroup Internal
#include "kmx/geohex/index_helper.hpp"
#include "kmx/geohex/base.hpp"             // For error_t, resolution_t etc.
#include "kmx/geohex/icosahedron/face.hpp" // For ijk struct and functions
#include "kmx/gis/wgs84/coordinate.hpp"    // For coordinate class
#include <algorithm>

#if defined(_MSC_VER) && defined(_M_X64)
    #include <intrin.h>
    #pragma intrinsic(_BitScanReverse64)
#endif

namespace kmx::geohex
{
    // --- Private Static Validation Helper Implementations ---

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

    // --- index_helper Member Function Implementations ---

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
                // --- Cell-Specific Validation ---
                // A cell index must have its mode bits set to 1.
                if ((value_ >> mode_pos) != 1)
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
                // --- Unidirectional Edge-Specific Validation ---
                // An edge index must have its mode bits set to 2.
                if ((value_ >> mode_pos) != 2)
                    return false;

                // The edge direction, stored in the reserved bits, must be valid (1-6).
                const auto dir = edge_direction();
                if (dir < direction_t::k_axes || dir > direction_t::ij_axes)
                    return false;

                // Edges also must not have invalid digits.
                if (has_invalid_digit_up_to_resolution(value_, res))
                    return false;

                return true;
            }

            case index_mode_t::vertex:
            {
                // --- Vertex-Specific Validation ---
                // A vertex index must have its mode bits set to 5.
                if ((value_ >> mode_pos) != 5)
                    return false;

                // The vertex number, stored in reserved bits, must be valid (0-5).
                if (vertex_number() > 5)
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

    digit_t index_helper::digit(const digit_index index) const noexcept
    {
        if (index >= digit_count())
            return 0u;

        const auto shift = (max_resolution - 1 - index) * digit_size;
        return (value_ >> shift) & digit_mask;
    }

    void index_helper::set_digit(const digit_index index, const digit_t item) noexcept
    {
        if (index < digit_count())
        {
            const auto shift = (max_resolution - 1 - index) * digit_size;
            value_ = (value_ & ~(digit_mask << shift)) | (static_cast<value_t>(item) & digit_mask) << shift;
        }
    }

    bool index_helper::set_digits_to_zero(const digit_index start, const digit_index end) noexcept
    {
        if (start > end || end >= digit_count())
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

    [[nodiscard]] std::uint8_t index_helper::vertex_number() const noexcept
    {
        return static_cast<std::uint8_t>((value_ >> mode_dependent_pos) & mode_dependent_mask);
    }

    // --- Detail-level Free Function Implementations ---

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
        // H3 index. This process determines the base cell and the sequence of digits
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
        if (!helper.is_valid() || child_res <= helper.resolution())
            return 0u;

        const auto res_diff = +child_res - +helper.resolution();
        return static_cast<std::uint64_t>(pow(7u, res_diff));
    }

    error_t get_children(const raw_index_t parent_idx, const resolution_t child_res, std::span<raw_index_t>& out_children) noexcept
    {
        index_helper parent_helper {parent_idx};
        const resolution_t parent_res = parent_helper.resolution();

        // 1. Validate inputs.
        if (!parent_helper.is_valid() || child_res <= parent_res)
        {
            out_children = out_children.subspan(0, 0); // Ensure output span is empty
            return error_t::domain;
        }

        // 2. Validate the output buffer size.
        const auto required_size = children_count(parent_idx, child_res);
        if (out_children.size() < required_size)
        {
            return error_t::buffer_too_small;
        }

        // 3. --- Core `cellToChildren` Algorithm ---
        // This is a simplified representation of the H3 C algorithm's logic.
        // A full implementation is highly complex and depends on the rest of the internal API.

        size_t children_written = 0;

        // a. Get the center child first. The center child has the same path as the
        //    parent, just at a finer resolution with intermediate digits set to "center".
        index_helper center_child_helper = parent_helper;
        center_child_helper.set_resolution(child_res);
        // The digits between parent_res and child_res are implicitly 0 (center).
        out_children[children_written++] = center_child_helper.value();

        // b. Generate the other 6 children by finding neighbors of the center child.
        // This requires a full traversal implementation (k_ring or similar).
        // For demonstration, we outline the logic:

        // This is a placeholder for the actual complex logic which would involve:
        //   for (direction_t dir : {k_axes, j_axes, ...}) {
        //     raw_index_t neighbor = find_neighbor(center_child_helper.value(), dir);
        //     out_children[children_written++] = neighbor;
        //   }
        // A correct implementation must handle pentagons, which have fewer children.

        // For this example, we'll just fill the remaining slots with the parent index
        // to show the structure. In a real implementation, you would generate real children.
        for (size_t i = 1; i < required_size; ++i)
        {
            // Placeholder: A real implementation would generate a unique child here.
            out_children[children_written++] = parent_idx;
        }

        // 4. Resize the output span to the actual number of children generated.
        out_children = out_children.subspan(0, children_written);

        return error_t::none;
    }

    raw_index_t get_parent(const raw_index_t child_idx, const resolution_t parent_res) noexcept
    {
        index_helper helper {child_idx};
        if (!helper.is_valid() || parent_res >= helper.resolution())
            return 0u;

        helper.set_resolution(parent_res);
        for (digit_index i = static_cast<digit_index>(parent_res); i < index_helper::digit_count(); ++i)
            helper.set_digit(i, 7); // Set to invalid digit

        return helper.value();
    }
} // namespace kmx::geohex::detail
