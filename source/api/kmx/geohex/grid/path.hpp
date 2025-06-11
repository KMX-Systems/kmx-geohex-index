/// @file geohex/grid/path.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/index.hpp>
#endif

namespace kmx::geohex::grid::path
{
    index::vector cells(const index& from, const index& to);
    void cells(const index& from, const index& to, index::vector& items);
    void cells(const index& from, const index& to, index::span& items);
}
