#ifndef COLOR_H
#define COLOR_H

#include <string>
#include "rl_wrap.h"
using namespace rlw;

namespace ClassicLauncher
{
    class ColorClassic : public ::Color
    {
    public:

        // Default white
        ColorClassic()
            : ::Color{ 255, 255, 255, 255 } {};

        ColorClassic(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
            : ::Color{ red, green, blue, alpha } {};

        void Set(const ::Color& color)
        {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a;
        }

        bool operator==(const ::Color& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
        bool operator!=(const ::Color& other) const { return !(*this == other); }

        void SetColor(int red, int green, int blue)
        {
            if (red < 0)
            {
                red = 0;
            }
            else if (red > 255)
            {
                red = 255;
            }

            if (green < 0)
            {
                green = 0;
            }
            else if (green > 255)
            {
                green = 255;
            }
            if (blue < 0)
            {
                blue = 0;
            }
            else if (blue > 255)
            {
                blue = 255;
            }

            r = red;
            g = green;
            b = blue;
        }
        void SetColor(int red, int green, int blue, int alpha)
        {
            SetColor(red, green, blue);
            SetOpacity(alpha);
        }
        void SetOpacity(int alpha)
        {
            if (alpha < 0) alpha = 0;
            if (alpha > 255) alpha = 255;
            a = alpha;
        }

        int GetColorRed() const { return r; }
        int GetColorGreen() const { return g; }
        int GetColorBlue() const { return b; }
        int GetColorAlpha() const { return a; }

        ColorClassic GetColor() const { return *this; }

        std::string ToString() const { return TextFormat("Color(red: %d, green: %d, blue: %d, alpha: %d)", r, g, b, a); }

        static ColorClassic Yellow() { return ColorClassic{ 253, 249, 0, 255 }; }
        static ColorClassic Gold() { return ColorClassic{ 255, 203, 0, 255 }; }
        static ColorClassic Orange() { return ColorClassic{ 255, 161, 0, 255 }; }
        static ColorClassic Pink() { return ColorClassic{ 255, 109, 194, 255 }; }
        static ColorClassic Red() { return ColorClassic{ 230, 41, 55, 255 }; }
        static ColorClassic Maroon() { return ColorClassic{ 190, 33, 55, 255 }; }
        static ColorClassic Green() { return ColorClassic{ 0, 228, 48, 255 }; }
        static ColorClassic Lime() { return ColorClassic{ 0, 158, 47, 255 }; }
        static ColorClassic DarkGreen() { return ColorClassic{ 0, 117, 44, 255 }; }
        static ColorClassic SkyBlue() { return ColorClassic{ 102, 191, 255, 255 }; }
        static ColorClassic Blue() { return ColorClassic{ 0, 121, 241, 255 }; }
        static ColorClassic DarkBlue() { return ColorClassic{ 0, 82, 172, 255 }; }
        static ColorClassic Purple() { return ColorClassic{ 200, 122, 255, 255 }; }
        static ColorClassic Violet() { return ColorClassic{ 135, 60, 190, 255 }; }
        static ColorClassic DarkPurple() { return ColorClassic{ 112, 31, 126, 255 }; }
        static ColorClassic Beige() { return ColorClassic{ 211, 176, 131, 255 }; }
        static ColorClassic Brown() { return ColorClassic{ 127, 106, 79, 255 }; }
        static ColorClassic DarkBrown() { return ColorClassic{ 76, 63, 47, 255 }; }
        static ColorClassic White() { return ColorClassic{ 255, 255, 255, 255 }; }
        static ColorClassic Black() { return ColorClassic{ 0, 0, 0, 255 }; }
        static ColorClassic Blank() { return ColorClassic{ 0, 0, 0, 0 }; }
        static ColorClassic Magenta() { return ColorClassic{ 255, 0, 255, 255 }; }
        static ColorClassic RayWhite() { return ColorClassic{ 245, 245, 245, 255 }; }
        static ColorClassic Cyan() { return ColorClassic{ 0, 255, 255, 255 }; }
        static ColorClassic Transparent() { return ColorClassic{ 0, 0, 0, 0 }; }
    };

}  // namespace ClassicLauncher

#endif  // COLOR_H