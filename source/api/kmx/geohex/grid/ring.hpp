#pragma once
#ifndef PCH
    #include <h3plus/index.hpp>
#endif

namespace h3plus::grid::ring
{
    index::vector unsafe(const index& index, const k_distance k);
    void unsafe(const index& index, const k_distance k, index::vector& items);
    void unsafe(const index& index, const k_distance k, index::span& items);
}
