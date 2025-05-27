/// @file gis/wgs84/coordinate.hpp
#pragma once
#ifndef PCH
#endif

namespace kmx::gis::wgs84
{
    class coordinate
    {
    public:
        using vector = int;
        using span = int;

        constexpr coordinate(const double lat = {}, const double lon = {}): latitude {lat}, longitude {lon} {}
        constexpr coordinate(const coordinate&) = default;
        constexpr coordinate(coordinate&&) = default;

        // double latitude() const noexcept { return latitude_; }
        // double longitude() const noexcept { return longitude_; }

        double azimuth_to(const coordinate& /*item*/) const { return 0; }

        double latitude;
        double longitude;
    };
}
