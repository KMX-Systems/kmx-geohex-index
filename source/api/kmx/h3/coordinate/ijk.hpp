/// @file h3/coordinate/ijk.hpp
#pragma once
#ifndef PCH
    #include <kmx/h3/base.hpp>
    #include <kmx/h3/coordinate/ij.hpp>
    #include <kmx/math/vector.hpp>
    #include <span>
    #include <tuple>
    #include <vector>
#endif

namespace kmx::h3::coordinate
{
    using vector2 = kmx::math::vector2d;
    using index_as_tuple = std::tuple<int, int, int>;

    class ijk: public ij
    {
    public:
        using span = std::span<ijk>;
        using vector = std::vector<ijk>;

        constexpr ijk() noexcept = default;
        constexpr ijk(const ijk&) noexcept = default;
        constexpr ijk(ijk&&) noexcept = default;
        constexpr ijk(const value i, const value j, const value k = 0) noexcept: ij(i, j), k(k) {}
        template <typename T>
        constexpr ijk(const std::tuple<T, T, T>& item): ij(std::get<0>(item), std::get<1u>(item)), k(std::get<2u>(item))
        {
        }
        constexpr ijk(const value data[3u]) noexcept: ij(data), k(data[2u]) {}
        constexpr ijk(const ij& ij_item, const value k = 0) noexcept: ij(ij_item), k(k) {}

        static ijk from_cube_round(const double i, const double j, const double k);

        vector2 center() const noexcept;

        ijk operator+(const ijk& item) const noexcept;
        ijk operator-(const ijk& item) const noexcept;

        constexpr ijk& operator=(const ijk&) noexcept = default;
        constexpr ijk& operator=(ijk&&) noexcept = default;

        constexpr bool operator==(const ijk&) const noexcept = default;
        constexpr bool operator!=(const ijk&) const noexcept = default;

        bool operator==(const index_as_tuple& item) const noexcept;
        bool operator!=(const index_as_tuple& item) const noexcept;

        void scale(int factor) noexcept;
        ijk operator*(int factor) const noexcept;
        void operator*=(int factor) noexcept { scale(factor); }

        void operator+=(const ijk& item) noexcept;
        void operator-=(const ijk& item) noexcept;

        bool normalize_could_overflow() noexcept;
        void normalize() noexcept;
        direction_t to_digit() const noexcept;
        bool up_ap7_checked() noexcept;
        bool up_ap7r_checked() noexcept;
        void up_ap7() noexcept;
        void up_ap7r() noexcept;
        void down_ap7() noexcept;
        void down_ap7r() noexcept;
        void down_ap3() noexcept;
        void down_ap3r() noexcept;
        void to_neighbor(const direction_t digit);
        ijk neighbor(const direction_t digit) const;
        void rotate_60ccw() noexcept;
        void rotate_60cw() noexcept;
        int distance_to(const ijk& b) const noexcept;
        void to_ij(ij& ij) const noexcept;
        bool from_ij(const ijk& ij) noexcept;
        void to_cube() noexcept;
        void from_cube() noexcept;

        value scalar_sum() const noexcept;

        value k;

    private:
        void transform(ijk i_vec, ijk j_vec, ijk k_vec) noexcept;
    };

    constexpr ijk to_ijk(const direction_t direction) noexcept
    {
        using tuple = std::tuple<int8_t, int8_t, int8_t>;

        static constexpr std::array<tuple, direction_count> data {{
            {0, 0, 0}, // direction 0
            {0, 0, 1}, // direction 1
            {0, 1, 0}, // direction 2

            {0, 1, 1}, // direction 3
            {1, 0, 0}, // direction 4
            {1, 0, 1}, // direction 5

            {1, 1, 0} // direction 6
        }};

        return data[+direction];
    }
}
