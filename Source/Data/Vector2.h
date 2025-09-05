#ifndef VECTOR_2_H
#define VECTOR_2_H


namespace ClassicLauncher
{
    template <typename T>
    class Vector2
    {
    public:

        T x;  // Vector x component
        T y;  // Vector y component

        Vector2(T x, T y)
            : x(x), y(y)
        {
        }

        Vector2(T x)
            : Vector2{ x, 0 } {};

        Vector2()
            : Vector2{ 0, 0 } {};

        Vector2& operator=(const Vector2& vector)
        {
            Set(vector);
            return *this;
        }

        bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vector2& other) const { return !(*this == other); }
        Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
        Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }
        Vector2 operator*(T scalar) const { return { x * scalar, y * scalar }; }
        Vector2 operator/(T scalar) const { return { x / scalar, y / scalar }; }

        void SetX(T x) { this->x = x; }
        void SetY(T y) { this->y = y; }

        void Set(T x, T y)
        {
            this->x = x;
            this->y = y;
        }

        void Set(const Vector2& vec)
        {
            x = vec.x;
            y = vec.y;
        }

    };

    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;
    using Vector2i = Vector2<int>;
    using Vector2u = Vector2<unsigned int>;
    using Size = Vector2<unsigned int>;

}  // namespace ClassicLauncher

#endif  // VECTOR_2_H
