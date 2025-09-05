#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <string>
#include "rl_wrap.h"
#include "Data/Vector2.h"

namespace ClassicLauncher
{
    class RectangleClassic : public rlw::Rectangle
    {
    public:

        RectangleClassic(const rlw::Rectangle& rect)
            : rlw::Rectangle{ rect.x, rect.y, rect.width, rect.height } {};

        RectangleClassic(float x, float y, float width, float height)
            : rlw::Rectangle{ x, y, width, height } {};

        RectangleClassic(float x, float y, float width)
            : rlw::Rectangle{ x, y, width, 0 } {};

        RectangleClassic(float x, float y)
            : rlw::Rectangle{ x, y, 0, 0 } {};

        RectangleClassic(float x)
            : rlw::Rectangle{ x, 0, 0, 0 } {};

        RectangleClassic()
            : rlw::Rectangle{ 0, 0, 0, 0 } {};

        RectangleClassic(Vector2f position, Vector2f size)
            : rlw::Rectangle{ position.x, position.y, size.x, size.y } {};

        RectangleClassic(Vector2f size)
            : rlw::Rectangle{ 0, 0, size.x, size.y } {};
            
        void SetX(float x) { this->x = x; }
        void SetY(float y) { this->y = y; }
        void SetWidth(float width) { this->width = width; }
        void SetHeight(float height) { this->height = height; }

        RectangleClassic& operator=(const rlw::Rectangle& rect)
        {
            Set(rect);
            return *this;
        }

        void Set(rlw::Rectangle rect)
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
        void SetPosition(const Vector2f& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
        }
        void SetSize(float width, float height)
        {
            this->width = width;
            this->height = height;
        }


        std::string ToString() const
        {
            return "Rectangle( x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", width: " + std::to_string(width) + ", height: " + std::to_string(height) + ")";
        }
        int GetIntX() const { return static_cast<int>(x); }
        int GetIntY() const { return static_cast<int>(y); }
        int GetIntWidth() const { return static_cast<int>(width); }
        int GetIntHeight() const { return static_cast<int>(height); }
        Vector2f GetPosition() const { return { x, y }; }
    };

}  // namespace ClassicLauncher

#endif  // RECTANGLE_H