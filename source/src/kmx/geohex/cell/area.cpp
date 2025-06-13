/// @file src/kmx/geohex/cell/area.cpp
/// @ingroup Internal
#include "kmx/geohex/cell/area.hpp"
#include "kmx/geohex/cell/boundary.hpp"
#include <array>
#include <cmath>
#include <kmx/gis/wgs84/coordinate.hpp>
#include <span>

namespace kmx::geohex::cell::area
{
    constexpr double earth_radius_km = 6371.0088;
    constexpr double meters_per_km = 1000.0;

    error_t km2(const index cell, double& out) noexcept
    {
        out = 0.0;
        double area_rads2_val;
        error_t err = rad2(cell, area_rads2_val);
        if (err != error_t::none)
            return err;

        out = area_rads2_val * earth_radius_km * earth_radius_km;
        return error_t::none;
    }

    error_t m2(const index cell, double& out) noexcept
    {
        out = 0.0;
        double area_rads2_val;
        error_t err = rad2(cell, area_rads2_val);
        if (err != error_t::none)
            return err;

        out = area_rads2_val * (earth_radius_km * meters_per_km) * (earth_radius_km * meters_per_km);
        return error_t::none;
    }

    error_t rad2(const index cell, double& out) noexcept
    {
        std::array<gis::wgs84::coordinate, boundary::max_vertices> boundary_data;
        std::span<gis::wgs84::coordinate> boundary_data_span {boundary_data.begin(), boundary_data.end()};
        error_t boundary_err = boundary::get(cell, boundary_data_span);

        if (boundary_err != error_t::none)
            return boundary_err;

        if (boundary_data_span.size() < 3u)
            return error_t::none; // Area is 0 for degenerate polygons

        double area_sum {};
        for (std::uint8_t i {}; i < boundary_data_span.size(); ++i)
        {
            const auto& current_vert = boundary_data[i];
            const auto& next_vert = boundary_data[(i + 1u) % boundary_data_span.size()];
            const auto& prev_vert = boundary_data[(i + boundary_data_span.size() - 1u) % boundary_data_span.size()];
            area_sum += (next_vert.longitude - prev_vert.longitude) * std::sin(current_vert.latitude);
        }

        out = std::fabs(area_sum / 2.0);
        return error_t::none;
    }
}
