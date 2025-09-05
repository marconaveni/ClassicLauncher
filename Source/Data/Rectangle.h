#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <string>
#include "Data/Vector2.h"

namespace ClassicLauncher
{

    template <typename T>
    class Rectangle
    {
    public:

        T x;       // position left rectangle
        T y;       // position top rectangle
        T width;   // width rectangle
        T height;  // height rectangle

        constexpr Rectangle()
            : x(0), y(0), width(0), height(0) {};

        constexpr Rectangle(const Rectangle& rect)
            : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {};

        constexpr Rectangle(const Vector2<T>& position, const Vector2<T>& size)
            : x(position.x), y(position.y), width(size.x), height(size.y) {};

        constexpr Rectangle(T x, T y, T width, T height)
            : x(x), y(y), width(width), height(height) {};

        template <typename U>
        constexpr operator Rectangle<U>() const
        {
            return Rectangle<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(width), static_cast<U>(height));
        }

        constexpr bool operator==(const Rectangle& other) const { return x == other.x && y == other.y && width == other.width && height == other.height; }
        constexpr bool operator!=(const Rectangle& other) const { return !(*this == other); }

        void SetPosition(const Vector2<T>& position)
        {
            x = position.x;
            y = position.y;
        }
        void SetSize(const Vector2<T>& size)
        {
            width = size.x;
            height = size.y;
        }

        [[nodiscard]] Vector2<T> GetPosition() const { return { x, y }; }
        [[nodiscard]] Vector2<T> GetSize() const { return { width, height }; }
    };

    using RectFloat = Rectangle<float>;
    using RectInt = Rectangle<int>;

}  // namespace ClassicLauncher

#endif  // RECTANGLE_H