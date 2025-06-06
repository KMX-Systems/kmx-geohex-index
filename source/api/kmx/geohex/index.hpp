/// @file geohex/index.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/base.hpp>
    #include <span>
#endif

namespace kmx::geohex
{
    class index
    {
    public:
        using self = index;
        using value_t = std::uint64_t;
        using digit_index = std::uint8_t;
        using digit_t = char;

        constexpr index() noexcept = default;
        constexpr index(const index&) noexcept = default;
        constexpr index(index&&) noexcept = default;
        index(const value_t item) noexcept;

        bool is_valid() const noexcept;

        /// @ref isPentagon
        bool is_pentagon() const noexcept;

        index_mode_t mode() const noexcept;
        void set_mode(const index_mode_t item) noexcept;

        resolution_t resolution() const noexcept;
        void set_resolution(const resolution_t item) noexcept;

        /// @ref getBaseCellNumber
        cell::base::id_t base_cell() const noexcept;
        void set_base_cell(const cell::base::id_t item) noexcept;

        value_t value() const noexcept;
        void set_value(const value_t item) noexcept;

        value_t operator()() const noexcept;
        operator value_t() const noexcept;
        void operator=(const value_t item) noexcept;
        self& operator=(const self& item) noexcept = default;
        self& operator=(self&& item) noexcept = default;

        bool operator<(const index& item) const noexcept;
        bool operator<=(const index& item) const noexcept;
        bool operator>(const index& item) const noexcept;
        bool operator>=(const index& item) const noexcept;
        bool operator==(const index& item) const noexcept;
        bool operator!=(const index& item) const noexcept;

        static constexpr digit_index digit_count() noexcept { return 15u; }

        digit_t digit(const digit_index index) const noexcept;
        void set_digit(const digit_index index, const digit_t item) noexcept;

        /// @ref _zeroIndexDigits
        bool set_digits_to_zero(const digit_index start, const digit_index end) noexcept;

        /// @ref _h3LeadingNonZeroDigit
        direction_t leading_non_zero_digit() const noexcept;

        using number_span = std::span<char, 16u>;

        void get_number(number_span& span) const noexcept;

        /// @ref maxFaceCount
        std::uint8_t max_face_intersection_count() const noexcept { return is_pentagon() ? 5u : 2u; }

    private:
        static constexpr std::uint32_t shift(const digit_index index) noexcept { return field_digits_size - digit_size * (index + 1u); }
        static bool has_good_top_bits(const value_t h) noexcept;
        static bool has_any_7_up_to_resolution(const value_t h, const resolution_t res) noexcept;
        static bool has_all_7_after_resolution(const value_t h, const resolution_t ress) noexcept;
        static bool has_deleted_subsequence(const value_t h, cell::base::id_t base_cell);
        static uint32_t first_one_index(const value_t h) noexcept;

        static constexpr std::uint8_t digit_size = 3u;
        static constexpr value_t digit_mask = (1u << digit_size) - 1u;
        static constexpr std::uint8_t field_digits_size = 45u;
        static constexpr std::uint8_t field_base_cell_size = 7u;
        static constexpr std::uint8_t field_resolution_count = 4u;
        static constexpr std::uint8_t field_mode_dependent_size = 3u;
        static constexpr std::uint8_t field_mode_size = 4u;
        static constexpr std::uint8_t field_reserved_size = 1u;

        digit_t raw_digit(const digit_index index) const noexcept;

        [[maybe_unused]] value_t digits_: field_digits_size;
        value_t base_cell_: field_base_cell_size;
        value_t resolution_: field_resolution_count;
        [[maybe_unused]] value_t mode_dependent_: field_mode_dependent_size;
        value_t mode_: field_mode_size;
        [[maybe_unused]] value_t reserved_: field_reserved_size;
    };

    static_assert(sizeof(index) == 8u);
}
