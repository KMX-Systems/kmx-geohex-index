/// @file h3/cell/area.hpp
#pragma once
#ifndef PCH
    #include <kmx/h3/base.hpp>
#endif

namespace kmx::h3::cell::area
{
    /// @ref cellAreaKm2
    error_t km2(const index& cell, double& out) noexcept;

    /// @ref cellAreaM2
    error_t m2(const index& cell, double& out) noexcept;

    /// @ref cellAreaRads2
    error_t rad2(const index& cell, double& out) noexcept;
}
