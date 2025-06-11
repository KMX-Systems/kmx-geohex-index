/// @file geohex/grid/ring.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
#endif

namespace kmx::geohex::grid::ring
{
    index::vector unsafe(const index& index, const k_distance k);
    void unsafe(const index& index, const k_distance k, index::vector& items);
    void unsafe(const index& index, const k_distance k, index::span& items);
}
