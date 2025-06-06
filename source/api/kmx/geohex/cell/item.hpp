#pragma once
#ifndef PCH
    #include <h3plus/coordinate/wgs84.hpp>
    #include <h3plus/index.hpp>
#endif

namespace h3plus::cell
{
    class item: public index
    {
    public:
        using self = item;
        using vector = std::vector<self>;
        using span = std::span<self>;
        using child_position_t = std::uint8_t;
        using area_t = double;

        static std::uint32_t count(const resolution_t resolution) noexcept;

        using index::index;
        item(const coordinate::wgs84& coordinate, const resolution_t resolution);
        item(const index& origin, const coordinate::ij& origin2, const index_mode_t mode);

        virtual bool is_valid() const noexcept = 0;

        // virtual self parent() const = 0;
        virtual void get_parent(self& result) const = 0;

        virtual std::uint32_t children_count(const resolution_t children_resolution) const = 0;
        virtual vector children() const = 0;
        virtual void get_children(vector& items) const = 0;
        virtual void get_children(span& items) const = 0;

        // virtual self child(const child_position_t position, const resolution_t child_resolution) const;
        virtual void get_child(const child_position_t position, const resolution_t child_resolution, self& child_item) const;

        virtual child_position_t child_position(const resolution_t parent_resolution) const;

        // virtual self center_child(const resolution_t resolution) const;
        virtual void get_center_child(const resolution_t resolution, self& child_item) const;

        virtual area_t area_rads2() const;
        virtual area_t area_km2() const;
        virtual area_t area_m2() const;

        virtual coordinate::wgs84 center() const;
        virtual void get_center(coordinate::wgs84& output) const;

        virtual coordinate::wgs84::vector boundary() const;
        virtual void get_boundary(coordinate::wgs84::vector& output) const;
        virtual void get_boundary(coordinate::wgs84::span& output) const;

        virtual vector path_to(const self&) const;
        virtual coordinate::ij to_local_ij(const index& origin) const;
        virtual void to_local_ij(const index& origin, coordinate::ij& output) const;

    private:
    };
}

namespace h3plus::cell::resolution0
{
    constexpr std::uint32_t count() noexcept
    {
        return 122u;
    }

    item::vector items();
    void get_items(item::vector& output);
    static void get_items(item::span& output);
}
