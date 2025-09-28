#ifndef SIZE_H
#define SIZE_H


namespace ClassicLauncher
{
    template <typename T>
    class Vector2; // circular include todo: need is impl in .cpp or .inl


    template <typename T>
    class Size
    {
    public:

        T width;  // Vector x component
        T height; // Vector y component

        constexpr Size(T width, T height)
            : width(width), height(height)
        {
        }

        constexpr Size(T x)
            : width(x), height(x)
        {
        }

        constexpr Size(const Vector2<T>& v)
            : width(v.x), height(v.y)
        {
        }

        constexpr Size()
            : width(0), height(0)
        {
        }

        template <typename U>
        constexpr explicit operator Size<U>() const
        {
            return Size<U>(static_cast<U>(width), static_cast<U>(height));
        }

        constexpr bool operator==(const Size& other) const { return width == other.width && height == other.height; }
        constexpr bool operator!=(const Size& other) const { return !(*this == other); }
        constexpr Size operator+(const Size& other) const { return Size{width + other.width, height + other.height}; }
        constexpr Size operator-(const Size& other) const { return Size{width - other.width, height - other.height}; }
        constexpr Size operator*(T scalar) const { return {width * scalar, height * scalar}; }
        constexpr Size operator/(T scalar) const { return {width / scalar, height / scalar}; }


        constexpr Size<int> ToInt() const { return Size<int>(static_cast<int>(width), static_cast<int>(height)); }
        constexpr Size<float> ToFloat() const { return Size<float>(static_cast<float>(width), static_cast<float>(height)); }
        constexpr Size<double> ToDouble() const { return Size<double>(static_cast<double>(width), static_cast<double>(height)); }
    };

    using Sizef = Size<float>;
    using Sized = Size<double>;
    using Sizei = Size<int>;


} // namespace ClassicLauncher

#endif // SIZE_H
