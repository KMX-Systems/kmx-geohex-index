/// @file geohex/index.cpp
#include "kmx/geohex/index.hpp"
#include "kmx/geohex/cell/pentagon.hpp"
#include <algorithm>

namespace kmx::geohex
{
    index::index(const value_t item) noexcept
    {
        static_cast<void>(operator=(item));
    }

    bool index::has_good_top_bits(const value_t h) noexcept
    {
        const auto hh = (h >> 56u);
        return hh == 0b1000;
    }

    bool index::has_any_7_up_to_resolution(const value_t h, const resolution_t res) noexcept
    {
        static constexpr uint64_t MHI = 0b100100100100100100100100100100100100100100100;
        static constexpr uint64_t MLO = MHI >> 2;

        const auto shift = 3u * (15u - +res);
        const auto hh = (h >> shift) << shift;
        const auto result = (hh & MHI & (~hh - MLO));
        return result != 0;
    }

    bool index::has_all_7_after_resolution(const value_t h, const resolution_t ress) noexcept
    {
        // NOTE: res check is needed because we can't shift by 64
        const auto res = +ress;
        if (res < resolution_count)
        {
            const auto shift = 19u + 3u * res;
            const auto hh = ~h;
            const auto rr = (hh << shift) >> shift;
            return rr == 0;
        }

        return true;
    }

    uint32_t index::first_one_index(const value_t h) noexcept
    {
#if defined(__GNUC__) || defined(__clang__)
        return 63u - __builtin_clzll(h);
#elif defined(_MSC_VER) && defined(_M_X64) // doesn't work on win32
        uint32_t index;
        _BitScanReverse64(&index, h);
        return static_cast<uint32_t>(index);
#else
        // Portable fallback
        uint32_t pos = 63 - 19;
        H3Index m = 1;
        while ((h & (m << pos)) == 0)
            pos--;
        return pos;
#endif
    }

    bool index::has_deleted_subsequence(const value_t h, cell::base::id_t base_cell)
    {
        if (cell::pentagon::check(base_cell))
        {
            const auto hh = (h << 19u) >> 19u;
            if (hh == 0)
                return false; // all zeros: res 15 pentagon
            return (first_one_index(h) % 3u) == 0;
        }

        return false;
    }

    bool index::is_valid() const noexcept
    {
        const auto v = value();
        return has_good_top_bits(v) && (base_cell_ < cell::base::count) && has_any_7_up_to_resolution(v, resolution()) &&
               !has_deleted_subsequence(v, base_cell_);
    }

    bool index::is_pentagon() const noexcept
    {
        return cell::pentagon::check(base_cell()) && (leading_non_zero_digit() == direction_t::center);
    }

    index_mode_t index::mode() const noexcept
    {
        return static_cast<index_mode_t>(mode_);
    }

    void index::set_mode(const index_mode_t item) noexcept
    {
        static constexpr cell::base::id_t mask = (1u << field_mode_size) - 1u;
        mode_ = +item & mask;
    }

    resolution_t index::resolution() const noexcept
    {
        return static_cast<resolution_t>(resolution_);
    }

    void index::set_resolution(const resolution_t item) noexcept
    {
        static constexpr cell::base::id_t mask = (1u << field_resolution_count) - 1u;
        resolution_ = +item & mask;
    }

    cell::base::id_t index::base_cell() const noexcept
    {
        return static_cast<cell::base::id_t>(base_cell_);
    }

    void index::set_base_cell(const cell::base::id_t item) noexcept
    {
        static constexpr cell::base::id_t mask = (1u << field_base_cell_size) - 1u;
        base_cell_ = item & mask;
    }

    index::value_t index::value() const noexcept
    {
        return reinterpret_cast<const value_t&>(*this);
    }

    void index::set_value(const value_t item) noexcept
    {
        reinterpret_cast<value_t&>(*this) = item;
    }

    index::value_t index::operator()() const noexcept
    {
        return value();
    }

    index::operator value_t() const noexcept
    {
        return value();
    }

    void index::operator=(const value_t item) noexcept
    {
        set_value(item);
    }

    bool index::operator<(const index& item) const noexcept
    {
        return value() < item.value();
    }

    bool index::operator<=(const index& item) const noexcept
    {
        return value() <= item.value();
    }

    bool index::operator>(const index& item) const noexcept
    {
        return value() > item.value();
    }

    bool index::operator>=(const index& item) const noexcept
    {
        return value() >= item.value();
    }

    bool index::operator==(const index& item) const noexcept
    {
        return value() == item.value();
    }

    bool index::operator!=(const index& item) const noexcept
    {
        return value() != item.value();
    }

    index::digit_t index::raw_digit(const digit_index index) const noexcept
    {
        return static_cast<digit_t>((value() >> shift(index)) & digit_mask);
    }

    index::digit_t index::digit(const digit_index index) const noexcept
    {
        return (index < digit_count()) ? raw_digit(index) : 0U;
    }

    void index::set_digit(const digit_index index, const digit_t item) noexcept
    {
        if (index < digit_count())
        {
            const auto shift_value = shift(index);
            const auto new_data = (value() & (~digit_mask << shift_value)) | ((item & digit_mask) << shift_value);
            set_value(new_data);
        }
    }

    bool index::set_digits_to_zero(const digit_index start, const digit_index end) noexcept
    {
        if ((start < digit_count()) && (end <= digit_count()))
        {
            auto d = ~digits_;
            d <<= digit_size * (end - start + 1u);
            d = ~d;
            d <<= digit_size * (resolution_count - end);
            digits_ = ~d;
            return true;
        }

        return {};
    }

    direction_t index::leading_non_zero_digit() const noexcept
    {
        const auto count = static_cast<digit_index>(resolution_);
        for (digit_index i {}; i != count; ++i)
        {
            const auto digit = raw_digit(i);
            if (digit != 0)
                return static_cast<direction_t>(digit);
        }

        return direction_t::center;
    }

    void index::get_number(number_span& span) const noexcept
    {
        auto dest = span.begin();
        const auto max_count = std::min<digit_index>(span.size(), digit_count());
        for (digit_index i {}; i != max_count; ++i, ++dest)
        {
            *dest = static_cast<char>('0' + raw_digit(i));
        }
    }
}
