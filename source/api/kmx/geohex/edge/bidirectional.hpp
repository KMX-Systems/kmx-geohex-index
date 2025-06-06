#pragma once
#ifndef PCH
    #include <h3plus/edge/item.hpp>
#endif

namespace h3plus::edge
{
    class bidirectional: public item
    {
    public:
        using vector = std::vector<bidirectional>;
        using span = std::span<bidirectional>;
        using self = bidirectional;

        using item::item;

    private:
    };
}
