#pragma once
#ifndef PCH
    #include <h3plus/index.hpp>
#endif

namespace h3plus::edge
{
    class item: public index
    {
    public:
        using vector = std::vector<item>;
        using span = std::span<item>;
        using self = item;

        using index::index;
        item(const index& origin, const index& destination);

        index origin() const;
        index destination() const;
        index to(const index& destination) const;

        vector to_cells() const;
        void to_cells(vector& destination) const;
        void to_cells(span& destination) const;

        vector boundary() const;
        void boundary(vector& output) const;
        void boundary(span& output) const;

        double length_km() const;
        double length_m() const;
        double length_radians() const;

        static vector of(const index& origin) const;
        static void of(const index& origin, vector& output) const;
        static void of(const index& origin, span& output) const;

    private:
    };
}
