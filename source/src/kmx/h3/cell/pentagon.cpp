/// @file h3/cell/pentagon.cpp
#include "kmx/h3/cell/pentagon.hpp"

namespace kmx::h3::cell::pentagon
{
    void get(const resolution_t resolution, std::span<index, count> out) noexcept
    {
        index tmp;
        tmp.set_mode(index_mode_t::cell);
        tmp.set_resolution(resolution);

        auto dest = out.begin();
        for (auto base_cell: indexes())
        {
            tmp.set_base_cell(base_cell);
            *dest = tmp;
            ++dest;
        }
    }
}
