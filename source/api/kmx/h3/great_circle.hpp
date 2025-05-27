#pragma once
#ifndef PCH
    #include <h3plus/coordinate/wgs84.hpp>
#endif

namespace h3plus::great_circle
{
    using location = coordinate::wgs84;

    double distance_km(const location& from, const location& to);
    double distance_m(const location& from, const location& to);
    double distance_rads(const location& from, const location& to);
}
