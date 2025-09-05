#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <string>

namespace ClassicLauncher
{
    //template <typename T>
    class Vector2Classic
    {
    public:

        float x;  // Vector x component
        float y;  // Vector y component

        Vector2Classic(float x, float y)
            : x(x), y(y)
        {
        }

        Vector2Classic(float x)
            : Vector2Classic{ x, 0 } {};

        Vector2Classic()
            : Vector2Classic{ 0, 0 } {};

        Vector2Classic& operator=(const Vector2Classic& vector)
        {
            Set(vector);
            return *this;
        }

        bool operator==(const Vector2Classic& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vector2Classic& other) const { return !(*this == other); }

        void SetX(float x) { this->x = x; }
        void SetY(float y) { this->y = y; }

        void Set(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        void Set(const Vector2Classic& vec)
        {
            x = vec.x;
            y = vec.y;
        }

        std::string ToString() const { return "Vector2(x: " + std::to_string(x) + ", y: " + std::to_string(y) + ")"; }
        int GetIntX() const { return static_cast<int>(x); }
        int GetIntY() const { return static_cast<int>(y); }
    };

}  // namespace ClassicLauncher

#endif  // VECTOR_2_H
