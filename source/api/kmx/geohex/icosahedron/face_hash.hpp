/// @file geohex/coordinate/ijk_hash.hpp
#pragma once
#ifndef PCH
    #include <functional> // For std::hash
    #include <kmx/geohex/icosahedron/face.hpp>
#endif

namespace std
{
    /// @brief Hash specialization for the full FaceIJK struct.
    template <>
    struct hash<kmx::geohex::icosahedron::face::ijk>
    {
        std::size_t operator()(const kmx::geohex::icosahedron::face::ijk& fijk) const noexcept
        {
#ifdef KMX_GEOHEX_SIMPLE_HASH
            const std::size_t h1 = std::hash<kmx::geohex::coordinate::ijk> {}(fijk.ijk_coords);
            const std::size_t h2 = std::hash<kmx::geohex::icosahedron::face::no_t> {}(+fijk.face);
            return h1 ^ (h2 << 1u);
#else
            std::size_t seed {};
            kmx::hash_combine(seed, fijk.ijk_coords);
            kmx::hash_combine(seed, +fijk.face);
            return seed;
#endif
        }
    };
}
