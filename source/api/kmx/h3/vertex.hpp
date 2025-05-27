#pragma once
#ifndef PCH
    #include <h3plus/coordinate/wgs84.hpp>
    #include <h3plus/index.hpp>
#endif

namespace h3plus
{
    class vertex: public index
    {
    public:
        using index::index;

        coordinate::wgs84 to_wgs84() const;
        void to_wgs84(coordinate::wgs84& out) const;

    private:
    };
}
