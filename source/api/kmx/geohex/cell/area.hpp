/// @file geohex/cell/area.hpp
#pragma once
#ifndef PCH
    #include <kmx/geohex/base.hpp>
#endif

namespace kmx::geohex::cell::area
{
    /// @ref cellAreaKm2
    error_t km2(const index& cell, double& out) noexcept;

    /// @ref cellAreaM2
    error_t m2(const index& cell, double& out) noexcept;

    /// @ref cellAreaRads2
    error_t rad2(const index& cell, double& out) noexcept;
}
