/// @file inc/kmx/geohex/index_helper.hpp
/// @ingroup Internal
#pragma once
#ifndef PCH
    #include <kmx/geohex/base.hpp>
    #include <span>
#endif

// Forward Declarations
namespace kmx::gis::wgs84
{
    class coordinate;
}

namespace kmx::geohex::icosahedron::face
{
    struct ijk;
    error_t from_index(uint64_t index, ijk& out_fijk) noexcept;
    error_t to_wgs(const ijk& fijk, resolution_t res, gis::wgs84::coordinate& out_coord) noexcept;
}

// End Forward Declarations

namespace kmx::geohex
{
    /// @brief Internal helper class for bit-level manipulation of an index.
    /// @note This class is an implementation detail and should not be used directly.
    class index_helper
    {
    public:
        using value_t = raw_index_t;

        /// @brief Default constructor, initializes value to 0.
        constexpr index_helper() noexcept = default;

        /// @brief Constructs a helper from a raw 64-bit index value.
        /// @param item The raw index value.
        explicit constexpr index_helper(const value_t item) noexcept: value_ {item} {}

        // This is a transient helper, so copy/move is disallowed.
        index_helper(const index_helper&) = default;
        index_helper(index_helper&&) = default;
        index_helper& operator=(const index_helper&) = default;
        index_helper& operator=(index_helper&&) = default;

        /// @brief Validates the index based on its bit pattern.
        /// @return True if the index is a valid cell, false otherwise.
        [[nodiscard]] bool is_valid() const noexcept;

        /// @brief Checks if the index is a pentagon.
        /// @return True if the index's base cell is a pentagon, false otherwise.
        [[nodiscard]] bool is_pentagon() const noexcept;

        /// @brief Gets the mode of the index.
        /// @return The index mode.
        [[nodiscard]] index_mode_t mode() const noexcept;

        /// @brief Sets the mode of the index.
        /// @param item The new index mode to set.
        void set_mode(index_mode_t item) noexcept;

        /// @brief Gets the resolution of the index.
        /// @return The resolution (0-15).
        [[nodiscard]] resolution_t resolution() const noexcept;

        /// @brief Sets the resolution of the index.
        /// @param item The new resolution to set (0-15).
        void set_resolution(resolution_t item) noexcept;

        /// @brief Gets the base cell of the index.
        /// @return The base cell (0-121).
        [[nodiscard]] cell::base::id_t base_cell() const noexcept;

        /// @brief Sets the base cell of the index.
        /// @param item The new base cell to set (0-121).
        void set_base_cell(cell::base::id_t item) noexcept;

        /// @brief Gets the raw 64-bit value of the index.
        /// @return The underlying uint64_t value.
        [[nodiscard]] constexpr value_t value() const noexcept { return value_; }

        /// @brief Returns the maximum number of resolution-specific digits.
        [[nodiscard]] static constexpr digit_index digit_count() noexcept { return 15u; }

        /// @brief Gets the resolution-specific digit at a given index.
        /// @param index The digit index (0-14). 0 is the most significant digit (res 1).
        /// @return The digit value (0-7). Returns 0 for out-of-bounds index.
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

        /// @brief Gets the direction of a unidirectional edge index.
        /// @note Only valid if mode() is edge_unidirectional.
        [[nodiscard]] direction_t edge_direction() const noexcept;

        /// @brief Encodes a logical edge direction into the mode-dependent bits of the raw index.
        /// @details This is a low-level "setter" for creating an edge-mode index. It uses a
        ///          "clear-then-set" bitwise pattern to modify only the mode-dependent bits.
        /// @note The caller is responsible for first calling `set_mode(index_mode_t::edge_unidirectional)`.
        /// @param direction The direction to encode (must be 1-6).
        void set_edge_direction(const direction_t direction) noexcept;

        /// @brief Gets the vertex number of a vertex index.
        /// @note Only valid if mode() is vertex.
        [[nodiscard]] std::uint8_t vertex_number() const noexcept;

        /// @brief Encodes a logical vertex number into the mode-dependent bits of the raw index.
        /// @details
        /// This is a low-level, high-performance "setter" function responsible for one part of
        /// creating a valid vertex-mode index. It operates directly on the 64-bit integer value.
        ///
        /// The function uses a "clear-then-set" bitwise pattern:
        /// 1. An inverted mask is used with a bitwise AND to zero out the 3 mode-dependent bits,
        ///    leaving all other bits (resolution, base cell, etc.) untouched.
        /// 2. The new vertex number is masked, shifted to the correct position, and then
        ///    merged into the raw index using a bitwise OR.
        ///
        /// @note
        /// This is a critical internal-only function. It performs **no validation** on the
        /// current mode of the index. The caller is responsible for first calling `set_mode()`
        /// to `index_mode_t::vertex` before using this function. It blindly overwrites the
        /// bits at the `mode_dependent_pos`, which could corrupt an edge index if misused.
        ///
        /// @param vertex_no The logical vertex number to encode. The valid range is [0-5].
        ///                   While the internal masking handles larger values, callers should
        ///                   always provide a value within the valid range.
        ///
        /// @see vertex::from_cell, index_helper::set_mode, index_helper::vertex_number
        void set_vertex_number(const vertex_no_t vertex_no) noexcept;

    private:
        static std::uint8_t shift_from(const digit_index index) noexcept;

        // Bitfield Layout Constants (snake_lower_case)
        static constexpr std::uint8_t reserved_pos = 63u;
        static constexpr std::uint8_t mode_pos = 59u;
        static constexpr std::uint8_t mode_dependent_pos = 56u;
        static constexpr std::uint8_t resolution_pos = 52u;
        static constexpr std::uint8_t base_cell_pos = 45u;

        static constexpr value_t reserved_mask = 0b1ull;
        static constexpr value_t mode_mask = 0b1111ull;
        static constexpr value_t mode_dependent_mask = 0b111ull;
        static constexpr value_t resolution_mask = 0b1111ull;
        static constexpr value_t base_cell_mask = 0b1111111ull;

        static constexpr std::uint8_t digit_size = 3u;
        static constexpr value_t digit_mask = 0b111ull;
        static constexpr std::uint8_t max_resolution = 15u;

        // Bitfield constants for edge/vertex data in reserved bits
        static constexpr std::uint8_t reserved_bits_pos = 0u;
        static constexpr value_t reserved_bits_mask = 0b111ull; // Assuming 3 bits for this data

        // Private Validation Helpers
        [[nodiscard]] static bool has_invalid_digit_up_to_resolution(value_t h, resolution_t res) noexcept;
        [[nodiscard]] static bool has_deleted_subsequence(value_t h, cell::base::id_t base_cell) noexcept;
        [[nodiscard]] static uint32_t first_one_index(value_t h) noexcept;

        value_t value_;
    };

    // Detail-level Free Functions
    /// @brief Finds the geographic center of a given index.
    /// @details This function serves as a low-level procedural wrapper for converting a raw
    ///          index into a WGS84 coordinate. It orchestrates the internal conversion
    ///          pipeline: index -> FaceIJK -> 3D Vector -> WGS84 Coordinate.
    /// @ref h3ToGeo
    /// @param idx The raw 64-bit index to convert.
    /// @param[out] out_coord The structure to be filled with the center coordinate.
    ///                       Latitude and longitude values will be in radians.
    /// @return `error_t::none` on success, or an error code if the index is invalid
    ///         or the conversion fails.
    [[nodiscard]] error_t to_wgs(const raw_index_t idx, gis::wgs84::coordinate& out_coord) noexcept;

    /// @brief Finds the index of the cell containing a given geographic coordinate.
    /// @details This is the primary entry point for converting geographic coordinates into
    ///          a raw index. It performs the full `geoToH3` algorithm by first finding
    ///          the appropriate icosahedron face and IJK coordinates, then converting that
    ///          internal representation into the final 64-bit index.
    /// @ref geoToH3
    /// @param coord The WGS84 geographic coordinate, with latitude and longitude in radians.
    /// @param res The target resolution (from `resolution_t::r0` to `resolution_t::r15`).
    /// @return The raw 64-bit index. Returns 0 (an invalid index) on failure or if
    ///         the input coordinate is invalid.
    [[nodiscard]] raw_index_t from_wgs(const gis::wgs84::coordinate& coord, const resolution_t res) noexcept;

    /// @brief Calculates the number of children an cell has at a finer resolution.
    /// @details The number of children is determined by the aperture of the grid (7). This
    ///          function correctly handles the reduced number of children for pentagonal
    ///          cells, which have 5 or 6 children at resolution 1 depending on their
    ///          orientation, and a derived number at finer resolutions.
    /// @ref cellToChildrenSize
    /// @param parent_idx The raw 64-bit index of the parent cell.
    /// @param child_res The desired resolution for the children (must be greater than the
    ///                  parent's resolution).
    /// @return The total number of children. Returns 0 if the input is invalid or the
    ///         child resolution is not greater than the parent's.
    [[nodiscard]] std::uint64_t children_count(const raw_index_t parent_idx, const resolution_t child_res) noexcept;

    /// @brief Gets the indexes of all children of a parent cell at a specified finer resolution.
    /// @details This is the internal, non-allocating worker function. The caller must
    ///          provide a buffer of sufficient size.
    /// @ref cellToChildren
    /// @param parent_idx The raw 64-bit index of the parent cell.
    /// @param child_res The desired child resolution.
    /// @param[out] out_children A span that will be filled with the raw 64-bit child indexes.
    ///                        Its size will be adjusted to the actual number of children written.
    /// @return `error_t::none` on success.
    [[nodiscard]] error_t get_children(const raw_index_t parent_idx, const resolution_t child_res,
                                       std::span<raw_index_t>& out_children) noexcept;

    /// @brief Finds the parent of an cell at a specified coarser resolution.
    /// @details This is achieved by ascending the grid hierarchy, which involves zeroing out
    ///          the digits of the child index that are finer than the requested parent
    ///          resolution and updating the resolution bits accordingly.
    /// @ref cellToParent
    /// @param child_idx The raw 64-bit index of the child cell.
    /// @param parent_res The desired parent resolution (must be less than the child's resolution).
    /// @return The raw 64-bit index of the parent. Returns 0 (an invalid index) if the
    ///         input is invalid or the parent resolution is not coarser than the child's.
    [[nodiscard]] raw_index_t get_parent(const raw_index_t child_idx, const resolution_t parent_res) noexcept;

} // namespace kmx::geohex
