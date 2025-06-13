/// @file api/kmx/geohex/index.hpp
/// @ingroup API
#pragma once
#ifndef PCH
    #include <cstddef>
    #include <cstdint>
    #include <kmx/geohex/base.hpp>
    #include <span>
    #include <string_view>
#endif

namespace kmx::gis::wgs84
{
    class coordinate;
}

namespace kmx::geohex::cell::boundary
{
    /// @brief Maximum number of vertices a single H3 cell can have.
    constexpr std::uint8_t max_vertices = 10u;
}

namespace kmx::geohex
{
    /// @brief Represents a 64-bit H3 hexagonal grid system index.
    class index
    {
    public:
        using self = index;
        using value_t = raw_index_t;

        using wgs_coord = gis::wgs84::coordinate;

        // --- Constructors and Assignment ---

        /// @brief Default constructor. Creates an invalid index with a value of 0.
        constexpr index() noexcept = default;

        /// @brief Copy constructor.
        constexpr index(const index&) noexcept = default;

        /// @brief Move constructor.
        constexpr index(index&&) noexcept = default;

        /// @brief Constructs an index from a raw 64-bit integer value.
        /// @param item The raw H3 index value.
        explicit constexpr index(const value_t item) noexcept: value_ {item} {}

        /// @brief Copy assignment operator.
        self& operator=(const self&) noexcept = default;

        /// @brief Move assignment operator.
        self& operator=(self&&) noexcept = default;

        /// @brief Assigns a raw 64-bit integer value to this index.
        /// @param item The raw H3 index value to assign.
        void operator=(const value_t item) noexcept { value_ = item; }

        // --- Value Access ---

        /// @brief Gets the raw 64-bit integer value of the index.
        /// @return The underlying uint64_t value.
        [[nodiscard]] constexpr value_t value() const noexcept { return value_; }

        /// @brief Sets the raw 64-bit integer value of the index.
        /// @param item The new raw H3 index value.
        void set_value(const value_t item) noexcept { value_ = item; }

        /// @brief Function-call operator to get the raw value.
        /// @return The underlying uint64_t value.
        [[nodiscard]] constexpr value_t operator()() const noexcept { return value_; }

        /// @brief Implicit conversion to the raw 64-bit integer value.
        [[nodiscard]] constexpr operator value_t() const noexcept { return value_; }

        // --- Comparison ---

        /// @brief Non-equality comparison operator.
        [[nodiscard]] bool operator!=(const index& other) const noexcept = default;

        /// @brief Equality comparison operator.
        [[nodiscard]] bool operator==(const index& other) const noexcept = default;

        // --- Properties ---

        /// @brief Validates the H3 index.
        /// @return True if the index represents a valid H3 cell, false otherwise.
        [[nodiscard]] bool is_valid() const noexcept;

        /// @brief Checks if the H3 index is one of the 12 pentagons.
        /// @return True if the index corresponds to a pentagonal cell, false otherwise.
        [[nodiscard]] bool is_pentagon() const noexcept;

        /// @brief Gets the mode of the H3 index.
        /// @return The H3 index mode.
        [[nodiscard]] index_mode_t mode() const noexcept;

        /// @brief Sets the mode of the H3 index.
        /// @param item The new H3 index mode to set.
        void set_mode(const index_mode_t item) noexcept;

        /// @brief Gets the resolution of the H3 index.
        /// @return The H3 resolution (0-15).
        [[nodiscard]] resolution_t resolution() const noexcept;

        /// @brief Sets the resolution of the H3 index.
        /// @param item The new resolution to set (0-15).
        void set_resolution(const resolution_t item) noexcept;

        /// @brief Gets the base cell of the H3 index.
        /// @return The H3 base cell (0-121).
        [[nodiscard]] cell::base::id_t base_cell() const noexcept;

        /// @brief Sets the base cell of the H3 index.
        /// @param item The new base cell to set (0-121).
        void set_base_cell(const cell::base::id_t item) noexcept;

        // --- Digit Manipulation ---

        /// @brief Returns the maximum number of resolution-specific digits.
        [[nodiscard]] static constexpr digit_index digit_count() noexcept { return 15u; }

        /// @brief Gets the resolution-specific digit at a given index.
        /// @param index The digit index (0-14).
        /// @return The digit value (0-7).
        [[nodiscard]] digit_t digit(const digit_index index) const noexcept;

        /// @brief Sets the resolution-specific digit at a given index.
        /// @param index The digit index (0-14) to modify.
        /// @param item The new digit value to set (0-7).
        void set_digit(const digit_index index, const digit_t item) noexcept;

        /// @brief Sets a range of resolution digits to 0.
        /// @param start The starting digit index (inclusive, 0-14).
        /// @param end The ending digit index (inclusive, 0-14).
        /// @return True on success, false if indices are out of bounds.
        bool set_digits_to_zero(const digit_index start, const digit_index end) noexcept;

        /// @brief Gets the first non-zero resolution digit.
        /// @return The leading non-zero digit, or Center for an all-zero index.
        [[nodiscard]] direction_t leading_non_zero_digit() const noexcept;

        // --- String/Number Representation ---

        /// @brief Fills a span with the character representation of the index digits.
        /// @param span The output span to fill. Must be of size 16. The last character will be '\0'.
        using number_span = std::span<char, 16u>;
        void get_number(number_span& span) const noexcept;

        // --- Geographic Functions ---

        /// @brief Calculates the area of this H3 cell in square kilometers.
        /// @param[out] out_area The calculated area in km^2.
        /// @return error_t::none on success, or an error code.
        [[nodiscard]] error_t get_area_km2(double& out_area) const noexcept;

        /// @brief Calculates the area of this H3 cell in square meters.
        /// @param[out] out_area The calculated area in m^2.
        /// @return error_t::none on success, or an error code.
        [[nodiscard]] error_t get_area_m2(double& out_area) const noexcept;

        /// @brief Gets the vertices that form the boundary of this H3 cell.
        /// @param[out] out A span to be filled with the boundary vertices.
        /// @return error_t::none on success, or an error code.
        [[nodiscard]] error_t get_boundary(std::span<wgs_coord>& out) const noexcept;

        /// @brief Finds the center WGS84 coordinate of this H3 index.
        /// @param[out] out_coord The structure to fill with the center coordinate.
        /// @return error_t::none on success, or an error code if the index is invalid.
        [[nodiscard]] error_t to_wgs(wgs_coord& out_coord) const noexcept;

        /// @brief Finds the H3 index of the cell containing a given WGS84 coordinate.
        /// @param coord The WGS84 geographic coordinate.
        /// @param res The target H3 resolution.
        /// @return The H3 index. Returns an invalid index (value 0) on error.
        [[nodiscard]] static index from_wgs(const wgs_coord& coord, const resolution_t res) noexcept;

        // --- Hierarchy Functions ---

        /// @brief Calculates the number of children this cell has at a finer resolution.
        /// @param child_resolution The resolution of the children.
        /// @return The number of children. Returns 0 on error.
        [[nodiscard]] std::uint64_t children_count(const resolution_t child_resolution) const noexcept;

        /// @brief Gets the indexes of all children of this cell at a finer resolution.
        /// @details This is a non-allocating function. The caller must provide a span
        ///          large enough to hold all the children. The required size can be
        ///          determined by calling `children_count()`.
        /// @ref cellToChildren
        /// @param child_resolution The desired child resolution (must be > this->resolution()).
        /// @param[out] out_children A span to be filled with the child indexes. Its size
        ///                        will be adjusted to the actual number of children written.
        /// @return `error_t::none` on success, or an error code on failure.
        [[nodiscard]] error_t get_children(const resolution_t child_resolution, std::span<index>& out_children) const noexcept;

        /// @brief Gets the parent of this cell at a coarser resolution.
        /// @param parent_resolution The desired parent resolution.
        /// @return The parent H3 index. Returns an invalid index on error.
        [[nodiscard]] index get_parent(const resolution_t parent_resolution) const noexcept;

        /// @brief The maximum number of characters in the hexadecimal representation of an H3 index.
        /// @details A 64-bit integer requires at most 16 hex characters.
        static constexpr std::uint8_t max_hex_string_length = 16;

        /// @brief The required buffer size for string conversion, including a null terminator.
        static constexpr std::uint8_t max_hex_string_buffer_size = max_hex_string_length + 1u;

        /// @brief Converts an H3 index to its canonical hexadecimal string representation.
        /// @details This is a non-allocating function that uses `snprintf` for safe
        //           and efficient formatting. The caller must provide a buffer of
        //           sufficient size. The output span is resized to the actual number
        //           of characters written (excluding the null terminator).
        /// @ref h3ToString
        /// @param idx The H3 index to convert.
        /// @param[out] out_buffer A span of characters to write the result into.
        ///                        Must have a size of at least `max_hex_string_buffer_size`.
        /// @return `error_t::none` on success, or an error code.
        error_t to_string(index idx, std::span<char>& out_buffer) noexcept;

        /// @brief Parses a hexadecimal string representation to create an H3 index.
        /// @details This is a non-allocating function that uses `std::from_chars`,
        ///          the C++17 standard for high-performance, locale-independent parsing.
        /// @ref stringToH3
        /// @param str A string_view containing the hexadecimal representation of an H3 index.
        /// @return The parsed H3 index. Returns an invalid index (value 0) if parsing fails.
        [[nodiscard]] index from_string(std::string_view str) noexcept;

    private:
        value_t value_;
    };

} // namespace kmx::geohex

namespace std
{
    template <>
    struct hash<kmx::geohex::index>
    {
        std::size_t operator()(const kmx::geohex::index& idx) const noexcept
        {
            return std::hash<kmx::geohex::index::value_t> {}(idx.value());
        }
    };
}
