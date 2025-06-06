#pragma once
#ifndef PCH
    #include <h3plus/cell/item.hpp>
#endif

namespace h3plus::polygon::span_based
{
    using item = coordinate::wgs84::span;
    using vector = std::vector<item>;
    using span = std::span<item>;

    void to_cells(const item& polygon, const resolution_t resolution, cell::item::vector& cells);
    void to_cells(const item& polygon, const item& holes, const resolution_t resolution, cell::item::vector& cells);

    void to_cells(const item& polygon, const resolution_t resolution, cell::item::span& cells);
    void to_cells(const item& polygon, const item& holes, const resolution_t resolution, cell::item::span& cells);

    void cells_to_multi_polygon(const cell::item::vector& hexagons, vector& polygons);
    void cells_to_multi_polygon(const cell::item::span& hexagons, vector& polygons);
}
