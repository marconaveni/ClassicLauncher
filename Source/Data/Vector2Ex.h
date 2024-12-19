#ifndef VECTOR_2_EX_H
#define VECTOR_2_EX_H

#include <string>
#include "raylib.h"

namespace ClassicLauncher
{
    class RectangleEx : public ::Vector2
    {

    public:

        RectangleEx(const Vector2& vec) : Vector2{ vec.x, vec.y } {}
        RectangleEx(float x, float y) : Vector2{ x, y } {}
        RectangleEx(float x) : Vector2{ x, 0 } {}
        RectangleEx() : Vector2{ 0, 0 } {}
        RectangleEx& operator=(const ::Vector2& vector)
        {
            Set(vector);
            return *this;
        }

        bool operator==(const ::Vector2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const ::Vector2& other) const { return !(*this == other); }

        void SetX(float x) { this->x = x; }
        void SetY(float y) { this->y = y; }

        void Set(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        void Set(const ::Vector2& vec)
        {
            x = vec.x;
            y = vec.y;
        }

        std::string ToString() const { return "Vector2Ex(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }
        int GetIntX() const { return static_cast<int>(x); }
        int GetIntY() const { return static_cast<int>(y); }
    };

}  // namespace ClassicLauncher

#endif  // VECTOR_2_EX_H
