/// @file src/kmx/geohex/index.cpp
/// @ingroup Internal
#include "kmx/geohex/index.hpp"
#include "kmx/geohex/cell/area.hpp"
#include "kmx/geohex/cell/boundary.hpp"
#include "kmx/geohex/index_helper.hpp"
#include <algorithm> // For std::transform
#include <charconv>
#include <cstdio>

namespace kmx::geohex
{
    // --- Properties ---

    bool index::is_valid() const noexcept
    {
        return index_helper {value_}.is_valid();
    }

    bool index::is_pentagon() const noexcept
    {
        return index_helper {value_}.is_pentagon();
    }

    index_mode_t index::mode() const noexcept
    {
        return index_helper {value_}.mode();
    }

    void index::set_mode(const index_mode_t item) noexcept
    {
        index_helper helper {value_};
        helper.set_mode(item);
        value_ = helper.value();
    }

    resolution_t index::resolution() const noexcept
    {
        return index_helper {value_}.resolution();
    }

    void index::set_resolution(const resolution_t item) noexcept
    {
        index_helper helper {value_};
        helper.set_resolution(item);
        value_ = helper.value();
    }

    cell::base::id_t index::base_cell() const noexcept
    {
        return index_helper {value_}.base_cell();
    }

    void index::set_base_cell(const cell::base::id_t item) noexcept
    {
        index_helper helper {value_};
        helper.set_base_cell(item);
        value_ = helper.value();
    }

    // --- Digit Manipulation ---

    digit_t index::digit(const digit_index index) const noexcept
    {
        return index_helper {value_}.digit(index);
    }

    void index::set_digit(const digit_index index, const digit_t item) noexcept
    {
        index_helper helper {value_};
        helper.set_digit(index, item);
        value_ = helper.value();
    }

    bool index::set_digits_to_zero(const digit_index start, const digit_index end) noexcept
    {
        index_helper helper {value_};
        const bool result = helper.set_digits_to_zero(start, end);
        value_ = helper.value();
        return result;
    }

    direction_t index::leading_non_zero_digit() const noexcept
    {
        return index_helper {value_}.leading_non_zero_digit();
    }

    void index::get_number(number_span& span) const noexcept
    {
        const index_helper helper {value_};
        for (digit_index i {}; i < digit_count(); ++i)
            span[i] = static_cast<char>('0' + helper.digit(i));

        span[digit_count()] = 0;
    }

    // --- Geographic Functions ---

    error_t index::get_area_km2(double& out_area) const noexcept
    {
        return cell::area::km2(*this, out_area);
    }

    error_t index::get_area_m2(double& out_area) const noexcept
    {
        return cell::area::m2(*this, out_area);
    }

    error_t index::get_boundary(std::span<wgs_coord>& out) const noexcept
    {
        return cell::boundary::get(*this, out);
    }

    error_t index::to_wgs(wgs_coord& out_coord) const noexcept
    {
        return geohex::to_wgs(value_, out_coord);
    }

    [[nodiscard]] index index::from_wgs(const wgs_coord& coord, const resolution_t res) noexcept
    {
        return index {geohex::from_wgs(coord, res)};
    }

    // --- Hierarchy Functions ---

    std::uint64_t index::children_count(const resolution_t child_resolution) const noexcept
    {
        return geohex::children_count(value_, child_resolution);
    }

    error_t index::get_children(const resolution_t child_resolution, std::span<index>& out_children) const noexcept
    {
        // 1. Determine the exact number of children this cell will have.
        const auto required_size = this->children_count(child_resolution);

        // 2. Validate that the caller's buffer is large enough. This is the core
        //    safety check of the span-based design.
        if (out_children.size() < required_size)
            return error_t::buffer_too_small;

        // 3. Create a span of raw_index_t that points to the same memory as the
        //    caller's span<index>. This is a highly efficient cast that avoids
        //    any copying, possible because `index` is a standard-layout class
        //    wrapping a single raw_index_t.
        std::span<raw_index_t> raw_children_span(reinterpret_cast<raw_index_t*>(out_children.data()), out_children.size());

        // 4. Call the internal worker function to do the actual calculation.
        //    This function will fill the buffer and resize the raw_children_span.
        const error_t err = geohex::get_children(value_, child_resolution, raw_children_span);

        // 5. If the internal function succeeded, resize the original caller's span
        //    to match the number of children that were actually written.
        if (err == error_t::none)
            out_children = out_children.subspan(0u, raw_children_span.size());

        return err;
    }

    index index::get_parent(const resolution_t parent_resolution) const noexcept
    {
        return index {geohex::get_parent(value_, parent_resolution)};
    }

    error_t index::to_string(index idx, std::span<char>& out_buffer) noexcept
    {
        // 1. Validate the output buffer size.
        if (out_buffer.size() < max_hex_string_buffer_size)
            return error_t::buffer_too_small;

        // 2. Use snprintf for safe, fast, and standard-compliant formatting.
        // It's often faster than iostreams and guarantees null termination.
        // The format specifier "%llx" is for an unsigned long long (uint64_t).
        const int chars_written = std::snprintf(out_buffer.data(), out_buffer.size(), "%llx", idx.value());

        // 3. Check for encoding errors.
        if ((chars_written < 0) || (static_cast<std::size_t>(chars_written) >= out_buffer.size()))
        {
            // Encoding error or truncation occurred. Clear the buffer to be safe.
            out_buffer.front() = 0;
            out_buffer = out_buffer.subspan(0u, 0u);
            return error_t::failed;
        }

        // 4. Update the caller's span to reflect the actual number of characters written.
        out_buffer = out_buffer.subspan(0, static_cast<std::size_t>(chars_written));

        return error_t::none;
    }

    [[nodiscard]] index index::from_string(std::string_view str) noexcept
    {
        // 1. Basic validation. An H3 string cannot be empty or too long.
        if (str.empty() || str.length() > max_hex_string_length)
            return index {0u};

        // 2. Use std::from_chars for non-allocating, non-throwing parsing.
        index::value_t value {};
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, 16);

        // 3. Check for successful parsing.
        // The entire string must be consumed (ptr == end) and there must be no error.
        if ((ec == std::errc()) && (ptr == str.data() + str.size()))
            return index {value};

        // 4. If parsing failed, return an invalid index.
        return index {0};
    }
} // namespace kmx::geohex
