/// @file geohex/coordinate/ijk_hash.hpp
#pragma once
#ifndef PCH
    #include <functional> // For std::hash
    #include <kmx/geohex/coordinate/ijk.hpp>
#endif

namespace std
{
    /// @brief Hash specialization for ijk coordinates, used for map keys.
    template <>
    struct hash<kmx::geohex::coordinate::ijk>
    {
        std::size_t operator()(const kmx::geohex::coordinate::ijk& c) const noexcept
        {
#ifdef KMX_GEOHEX_SIMPLE_HASH
            const std::size_t h1 = std::hash<decltype(c.i)> {}(c.i);
            const std::size_t h2 = std::hash<decltype(c.j)> {}(c.j);
            const std::size_t h3 = std::hash<decltype(c.k)> {}(c.k);
            // Combine hashes using a standard technique to reduce collisions.
            return h1 ^ (h2 << 1u) ^ (h3 << 2u);
#else
            std::size_t seed {};
            kmx::hash_combine(seed, c);
            return seed;
#endif
        }
    };
}
