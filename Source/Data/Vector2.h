#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <string>
#include "rl_wrap.h"
using namespace rlw;

namespace ClassicLauncher
{
    class Vector2Classic : public ::Vector2
    {
    public:

        Vector2Classic(const ::Vector2& vec)
            : ::Vector2{ vec.x, vec.y } {};

        Vector2Classic(float x, float y)
            : ::Vector2{ x, y } {};

        Vector2Classic(float x)
            : Vector2Classic{ x, 0 } {};

        Vector2Classic()
            : Vector2Classic{ 0, 0 } {};

        Vector2Classic& operator=(const ::Vector2& vector)
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

        std::string ToString() const { return "Vector2(x: " + std::to_string(x) + ", y: " + std::to_string(y) + ")"; }
        int GetIntX() const { return static_cast<int>(x); }
        int GetIntY() const { return static_cast<int>(y); }
    };

}  // namespace ClassicLauncher

#endif  // VECTOR_2_H
