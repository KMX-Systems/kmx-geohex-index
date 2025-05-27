#pragma once
#ifndef PCH
    #include <h3plus/base.hpp>
    #include <span>
    #include <vector>
#endif

namespace h3plus::coordinate
{
    using latitude_t = double;
    using longitude_t = double;

    class wgs84
    {
    public:
        using self = wgs84;
        using span = std::span<self>;
        using vector = std::vector<self>;

        wgs84() = default;
        wgs84(const latitude_t latitude, const longitude_t longitude);

        latitude_t latitude() const noexcept { return latitude_; }
        longitude_t longitude() const noexcept { return longitude_; }

        void set_latitude(const latitude_t item) noexcept { latitude_ = item; }
        void set_longitude(const longitude_t item) noexcept { longitude_ = item; }

    private:
        latitude_t latitude_ {};
        longitude_t longitude_ {};
    };
}
