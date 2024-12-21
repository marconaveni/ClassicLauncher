#ifndef COLOR_H
#define COLOR_H

#include <string>
#include "raylib.h"

namespace ClassicLauncher
{
    struct Color : public ::Color
    {
        // Default white
        Color()
            : ::Color{ 255, 255, 255, 255 } {};

        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
            : ::Color{ red, green, blue, alpha } {};

        void Set(const ::Color& color)
        {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a;
        }

        std::string ToString() const { return TextFormat("Color(red: %d, green: %d, blue: %d, alpha: %d)", r, g, b, a); }

        static Color LightGray() { return LIGHTGRAY; }
        static Color Gray() { return GRAY; }
        static Color DarkGray() { return DARKGRAY; }
        static Color Yellow() { return YELLOW; }
        static Color Gold() { return GOLD; }
        static Color Orange() { return ORANGE; }
        static Color Pink() { return PINK; }
        static Color Red() { return RED; }
        static Color Maroon() { return MAROON; }
        static Color Green() { return GREEN; }
        static Color Lime() { return LIME; }
        static Color DarkGreen() { return DARKGREEN; }
        static Color SkyBlue() { return SKYBLUE; }
        static Color Blue() { return BLUE; }
        static Color DarkBlue() { return DARKBLUE; }
        static Color Purple() { return PURPLE; }
        static Color Violet() { return VIOLET; }
        static Color DarkPurple() { return DARKPURPLE; }
        static Color Beige() { return BEIGE; }
        static Color Brown() { return BROWN; }
        static Color DarkBrown() { return DARKBROWN; }
        static Color White() { return WHITE; }
        static Color Black() { return BLACK; }
        static Color Blank() { return BLANK; }
        static Color Magenta() { return MAGENTA; }
        static Color RayWhite() { return RAYWHITE; }
    };

}  // namespace ClassicLauncher

#endif  // COLOR_H