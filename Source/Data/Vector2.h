#ifndef VECTOR_2_H
#define VECTOR_2_H

#include "Data/Size.h"

namespace ClassicLauncher
{
    template <typename T>
    class Vector2
    {
    public:

        T x; // Vector x component
        T y; // Vector y component

        constexpr Vector2(T x, T y)
            : x(x), y(y)
        {
        }

        constexpr Vector2(T x)
            : x(x), y(x)
        {
        }

        constexpr Vector2(const Size<T>& s)
            : x(s.width), y(s.height)
        {
        }

        constexpr Vector2()
            : x(0), y(0)
        {
        }

        template <typename U>
        constexpr explicit operator Vector2<U>() const
        {
            return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
        }

        constexpr bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
        constexpr bool operator!=(const Vector2& other) const { return !(*this == other); }
        constexpr Vector2 operator+(const Vector2& other) const { return Vector2{x + other.x, y + other.y}; }
        constexpr Vector2 operator-(const Vector2& other) const { return Vector2{x - other.x, y - other.y}; }
        constexpr Vector2 operator*(T scalar) const { return {x * scalar, y * scalar}; }
        constexpr Vector2 operator/(T scalar) const { return {x / scalar, y / scalar}; }

        constexpr Vector2<int> ToInt() const { return Vector2<int>(static_cast<int>(x), static_cast<int>(y)); }
        constexpr Vector2<float> ToFloat() const { return Vector2<float>(static_cast<float>(x), static_cast<float>(y)); }
        constexpr Vector2<double> ToDouble() const { return Vector2<double>(static_cast<double>(x), static_cast<double>(y)); }
    };

    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;
    using Vector2i = Vector2<int>;


} // namespace ClassicLauncher

#endif // VECTOR_2_H
