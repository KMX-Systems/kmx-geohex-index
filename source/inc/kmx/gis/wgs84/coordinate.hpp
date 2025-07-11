/// @file kmx/gis/wgs84/coordinate.hpp
#pragma once
#ifndef PCH
    #include <cmath>
    #include <numbers> // For std::numbers::pi_v
    #include <span>
    #include <vector>
#endif

namespace kmx::gis::wgs84
{
    /// @brief Represents a geographic coordinate in the WGS84 system.
    /// @details By convention within this library, all angular values (latitude, longitude)
    /// are stored and handled in radians unless a function is explicitly named
    /// with a `_degrees` suffix.
    class coordinate
    {
    public:
        // Type aliases for collections of coordinates.
        using vector = std::vector<coordinate>;
        using span = std::span<coordinate>;

        /// @brief Default constructor, initializes to (0, 0).
        constexpr coordinate() noexcept = default;

        /// @brief Constructs a coordinate from latitude and longitude values.
        /// @param lat_rad Latitude in radians.
        /// @param lon_rad Longitude in radians.
        constexpr coordinate(const double lat_rad, const double lon_rad) noexcept: latitude {lat_rad}, longitude {lon_rad} {}

        /// @brief Constructs a coordinate from a C-style array of [latitude, longitude].
        /// @param data An array where data[0] is latitude and data[1] is longitude, both in radians.
        constexpr coordinate(const double data[2u]) noexcept: latitude {data[0u]}, longitude {data[1u]} {}

        // Defaulted special members for copy and move semantics.
        constexpr coordinate(const coordinate&) noexcept = default;
        constexpr coordinate(coordinate&&) noexcept = default;
        constexpr coordinate& operator=(const coordinate&) noexcept = default;
        constexpr coordinate& operator=(coordinate&&) noexcept = default;

        /// @brief Creates a coordinate from degree values.
        /// @param lat_deg Latitude in degrees.
        /// @param lon_deg Longitude in degrees.
        /// @return A new coordinate object with values stored in radians.
        [[nodiscard]] static constexpr coordinate from_degrees(const double lat_deg, const double lon_deg) noexcept
        {
            constexpr double deg_to_rad = std::numbers::pi_v<double> / 180.0;
            return coordinate {lat_deg * deg_to_rad, lon_deg * deg_to_rad};
        }

        // --- GIS Calculations ---

        /// @brief Calculates the initial bearing (forward azimuth) from this point to another.
        /// @details The azimuth is the angle measured clockwise from the North direction line.
        /// @param dest The destination coordinate.
        /// @return The initial bearing in radians, in the range [-PI, PI].
        [[nodiscard]] double azimuth_to(const coordinate& dest) const noexcept
        {
            const double delta_lon = dest.longitude - this->longitude;
            const double cos_dest_lat = std::cos(dest.latitude);

            const double y = std::sin(delta_lon) * cos_dest_lat;
            const double x =
                std::cos(this->latitude) * std::sin(dest.latitude) - std::sin(this->latitude) * cos_dest_lat * std::cos(delta_lon);

            return std::atan2(y, x);
        }

        /// @brief Calculates the great-circle distance to another point using the Haversine formula.
        /// @details The Haversine formula is numerically well-conditioned for small distances.
        /// @param dest The destination coordinate.
        /// @param earth_radius The radius of the sphere (e.g., in kilometers or miles).
        /// @return The great-circle distance in the same units as `earth_radius`.
        [[nodiscard]] double haversine_distance_to(const coordinate& dest, const double earth_radius) const noexcept
        {
            const double delta_lat = dest.latitude - this->latitude;
            const double delta_lon = dest.longitude - this->longitude;

            const double a = std::sin(delta_lat / 2.0) * std::sin(delta_lat / 2.0) +
                             std::cos(this->latitude) * std::cos(dest.latitude) * std::sin(delta_lon / 2.0) * std::sin(delta_lon / 2.0);

            const double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

            return earth_radius * c;
        }

        /// @brief Calculates the great-circle distance to another point using the spherical law of cosines.
        /// @details This formula can be simpler but may suffer from rounding errors for small distances.
        /// It is generally suitable for most H3-related distance calculations.
        /// @param dest The destination coordinate.
        /// @param earth_radius The radius of the sphere (e.g., in kilometers or miles).
        /// @return The great-circle distance in the same units as `earth_radius`.
        [[nodiscard]] double distance_to(const coordinate& dest, const double earth_radius) const noexcept
        {
            const double central_angle =
                std::acos(std::sin(this->latitude) * std::sin(dest.latitude) +
                          std::cos(this->latitude) * std::cos(dest.latitude) * std::cos(dest.longitude - this->longitude));
            return earth_radius * central_angle;
        }

        // --- Public Data Members ---

        /// @brief Latitude in radians. Positive for North, negative for South.
        double latitude;

        /// @brief Longitude in radians. Positive for East, negative for West.
        double longitude;
    };

} // namespace kmx::gis::wgs84
