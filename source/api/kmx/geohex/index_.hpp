#pragma once
#ifndef PCH
    #include <cstdint>
    #include <span>
    #include <string>
    #include <string_view>
    #include <vector>

    #include <h3plus/base.hpp>
#endif

namespace h3plus
{
    using face = std::int8_t;
    using face_vector = std::vector<face>;
    using face_span = std::span<face>;
    using k_distance = std::uint32_t;

    class index
    {
    public:
        using vector = std::vector<index>;
        using span = std::span<index>;
        using self = index;

        index() = default;
        index(const std::uint64_t raw_index);
        index(const std::string& input);
        index(const std::string_view input);
        index(const index& item);
        index(index&& item);

        virtual ~index() = default;

        index_mode_t mode() const noexcept;

        bool is_valid() const noexcept;
        bool is_neighbor_of(const index& index) const;

        resolution_t resolution() const noexcept;
        int base_index() const noexcept;
        int base_cell_number() const noexcept;
        bool has_resolution_class3() const noexcept;

        vector indices_within_k_distance(const k_distance k) const;
        void get_indices_within_k_distance(const k_distance k, vector& items) const;
        void get_indices_within_k_distance(const k_distance k, span& items) const;

        std::int64_t grid_distance(const index& target) const;

    private:
        std::uint64_t data {};
    };

    std::string to_string(const index& index);
    void to_string(const index& index, std::string_view& output);
    void to_string(const index& index, std::string& output);
}
