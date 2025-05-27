#pragma once
#ifndef PCH
    #include <kmx/h3/base.hpp>
#endif

namespace kmx::h3
{
    constexpr double pi_180 = 0.0174532925199432957692369076848861271111;
    constexpr double _180_pi = 57.29577951308232087679815481410517033240547;

    namespace degree
    {
        constexpr double to_radian(const double value) noexcept
        {
            return value * pi_180;
        }
    }

    namespace radian
    {
        constexpr double to_degree(const double value) noexcept
        {
            return value * _180_pi;
        }
    }
}
