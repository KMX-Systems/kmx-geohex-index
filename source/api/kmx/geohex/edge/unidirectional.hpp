#pragma once
#ifndef PCH
    #include <h3plus/edge/item.hpp>
#endif

namespace h3plus::edge
{
    class unidirectional: public item
    {
    public:
        using vector = std::vector<unidirectional>;
        using span = std::span<unidirectional>;
        using self = unidirectional;

        using item::item;

    private:
    };
}
