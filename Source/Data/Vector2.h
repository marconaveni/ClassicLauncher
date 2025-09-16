#ifndef VECTOR_2_H
#define VECTOR_2_H


namespace ClassicLauncher
{
    template <typename T>
    class Vector2
    {
    public:

        T x; // Vector x component
        T y; // Vector y component

        constexpr explicit Vector2(T x, T y)
            : x(x), y(y)
        {
        }

        constexpr explicit Vector2(T x)
            : x(x), y(x)
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
        constexpr Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
        constexpr Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
        constexpr Vector2 operator*(T scalar) const { return {x * scalar, y * scalar}; }
        constexpr Vector2 operator/(T scalar) const { return {x / scalar, y / scalar}; }
    };

    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;
    using Vector2i = Vector2<int>;
    using Size = Vector2<int>;


} // namespace ClassicLauncher

#endif // VECTOR_2_H
