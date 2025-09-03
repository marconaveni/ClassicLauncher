#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <string>
#include "rl_wrap.h"
using namespace rlw;
#include "Data/Vector2.h"

namespace ClassicLauncher
{
    class RectangleClassic : public ::Rectangle
    {
    public:

        RectangleClassic(const ::Rectangle& rect)
            : ::Rectangle{ rect.x, rect.y, rect.width, rect.height } {};

        RectangleClassic(float x, float y, float width, float height)
            : ::Rectangle{ x, y, width, height } {};

        RectangleClassic(float x, float y, float width)
            : ::Rectangle{ x, y, width, 0 } {};

        RectangleClassic(float x, float y)
            : ::Rectangle{ x, y, 0, 0 } {};

        RectangleClassic(float x)
            : ::Rectangle{ x, 0, 0, 0 } {};

        RectangleClassic()
            : ::Rectangle{ 0, 0, 0, 0 } {};

        RectangleClassic(::Vector2 position, ::Vector2 size)
            : ::Rectangle{ position.x, position.y, size.x, size.y } {};

        RectangleClassic(::Vector2 size)
            : ::Rectangle{ 0, 0, size.x, size.y } {};

        RectangleClassic(::Vector4 rect)
            : ::Rectangle{ rect.x, rect.y, rect.z, rect.w } {};
            
        void SetX(float x) { this->x = x; }
        void SetY(float y) { this->y = y; }
        void SetWidth(float width) { this->width = width; }
        void SetHeight(float height) { this->height = height; }

        RectangleClassic& operator=(const ::Rectangle& rect)
        {
            Set(rect);
            return *this;
        }

        void Set(::Rectangle rect)
        {
            this->x = rect.x;
            this->y = rect.y;
            this->width = rect.width;
            this->height = rect.height;
        }
        void Set(float x, float y, float width, float height)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }

        void SetPosition(float x, float y)
        {
            this->x = x;
            this->y = y;
        }
        void SetPosition(const ::Vector2& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
        }
        void SetSize(float width, float height)
        {
            this->width = width;
            this->height = height;
        }
        void SetSize(const ::Vector2& vec)
        {
            this->width = vec.x;
            this->height = vec.y;
        }

        std::string ToString() const
        {
            return "Rectangle( x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", width: " + std::to_string(width) + ", height: " + std::to_string(height) + ")";
        }
        int GetIntX() const { return static_cast<int>(x); }
        int GetIntY() const { return static_cast<int>(y); }
        int GetIntWidth() const { return static_cast<int>(width); }
        int GetIntHeight() const { return static_cast<int>(height); }
        Vector2Classic GetPosition() const { return { x, y }; }
    };

}  // namespace ClassicLauncher

#endif  // RECTANGLE_H