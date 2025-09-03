#ifndef SIZE_H
#define SIZE_H

#include <string>
#include "Vector2.h"
#include "rl_wrap.h"
using namespace rlw;

namespace ClassicLauncher
{
    class Size
    {
    public:

        float width;
        float height;

        Size()
            : width(0), height(0) {};

        Size(float width, float height)
            : width(width), height(height) {};

        Size(float width)
            : width(width), height(0) {};

        Size()
            : Size{ 0, 0 } {};

        Size& operator=(const Size& size)
        {
            Set(size.width, size.height);
            return *this;
        }

        bool operator==(const Size& other) const { return width == other.width && height == other.height; }
        bool operator!=(const Size& other) const { return !(*this == other); }

        void SetWidth(float width) { this->width = width; }
        void SetHeight(float height) { this->height = height; }
        void Set(Size size) { Set(size.width, size.height); }
        void Set(float width, float height)
        {
            this->width = width;
            this->height = height;
        }

        std::string ToString() const { return "Size(width: " + std::to_string(width) + ", height: " + std::to_string(height) + ")"; }
        int GetIntWidth() const { return static_cast<int>(width); }
        int GetIntHeight() const { return static_cast<int>(height); }

        Vector2Classic ToVector() { return Vector2Classic(width, height); }
    };

}  // namespace ClassicLauncher

#endif  // SIZE_H