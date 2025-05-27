#pragma once
#ifndef PCH
    #include <h3plus/index.hpp>
#endif

namespace h3plus::grid::disk
{
    std::uint32_t max_size(const k_distance k) noexcept;

    index::vector safe(const index& index, const k_distance k);
    void safe(const index& index, const k_distance k, index::vector& items);
    void safe(const index& index, const k_distance k, index::span& items);

    index::vector unsafe(const index& index, const k_distance k);
    void unsafe(const index& index, const k_distance k, index::vector& items);
    void unsafe(const index& index, const k_distance k, index::span& items);

    namespace distance
    {
        void safe(const index& origin, const k_distance k, index::vector& items, std::vector<int>& distances);
        void safe(const index& origin, const k_distance k, index::span& items, std::span<int>& distances);

        void unsafe(const index& origin, const k_distance k, index::vector& items, std::vector<int>& distances);
        void unsafe(const index& origin, const k_distance k, index::span& items, std::span<int>& distances);
    }
}
