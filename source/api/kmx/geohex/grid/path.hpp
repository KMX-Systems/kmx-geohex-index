#pragma once
#ifndef PCH
    #include <h3plus/index.hpp>
#endif

namespace h3plus::grid::path
{
    index::vector cells(const index& from, const index& to);
    void cells(const index& from, const index& to, index::vector& items);
    void cells(const index& from, const index& to, index::span& items);
}
