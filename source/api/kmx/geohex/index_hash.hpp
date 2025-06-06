/// @file geohex/index_hash.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
    #include <unordered_map>
#endif

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
