#ifndef COLOR_H
#define COLOR_H

namespace ClassicLauncher
{
    class ColorClassic
    {
    public:

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;

        // Default white
        ColorClassic()
            : r(255), g(255), b(255), a(255) {};

        ColorClassic(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {};

        bool operator==(const ColorClassic& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
        bool operator!=(const ColorClassic& other) const { return !(*this == other); }

        void SetColor(unsigned char red, unsigned char green, unsigned char blue)
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



        static ColorClassic WhiteGray() { return ColorClassic{ 245, 245, 245, 255 }; }  // Gray (Almost White)
        static ColorClassic LightGray() { return ColorClassic{ 200, 200, 200, 255 }; }  // Light Gray
        static ColorClassic Gray() { return ColorClassic{ 130, 130, 130, 255 }; }       // Gray
        static ColorClassic DarkGray() { return ColorClassic{ 80, 80, 80, 255 }; }      // Dark Gray
        static ColorClassic Yellow() { return ColorClassic{ 253, 249, 0, 255 }; }       // Yellow
        static ColorClassic Gold() { return ColorClassic{ 255, 203, 0, 255 }; }         // Gold
        static ColorClassic Orange() { return ColorClassic{ 255, 161, 0, 255 }; }       // Orange
        static ColorClassic Pink() { return ColorClassic{ 255, 109, 194, 255 }; }       // Pink
        static ColorClassic Red() { return ColorClassic{ 230, 41, 55, 255 }; }          // Red
        static ColorClassic Maroon() { return ColorClassic{ 190, 33, 55, 255 }; }       // Maroon
        static ColorClassic Green() { return ColorClassic{ 0, 228, 48, 255 }; }         // Green
        static ColorClassic Lime() { return ColorClassic{ 0, 158, 47, 255 }; }          // Lime
        static ColorClassic DarkGreen() { return ColorClassic{ 0, 117, 44, 255 }; }     // Dark Green
        static ColorClassic SkyBlue() { return ColorClassic{ 102, 191, 255, 255 }; }    // Sky Blue
        static ColorClassic Blue() { return ColorClassic{ 0, 121, 241, 255 }; }         // Blue
        static ColorClassic DarkBlue() { return ColorClassic{ 0, 82, 172, 255 }; }      // Dark Blue
        static ColorClassic Purple() { return ColorClassic{ 200, 122, 255, 255 }; }     // Purple
        static ColorClassic Violet() { return ColorClassic{ 135, 60, 190, 255 }; }      // Violet
        static ColorClassic DarkPurple() { return ColorClassic{ 112, 31, 126, 255 }; }  // Dark Purple
        static ColorClassic Beige() { return ColorClassic{ 211, 176, 131, 255 }; }      // Beige
        static ColorClassic Brown() { return ColorClassic{ 127, 106, 79, 255 }; }       // Brown
        static ColorClassic DarkBrown() { return ColorClassic{ 76, 63, 47, 255 }; }     // DarkBrown
        static ColorClassic White() { return ColorClassic{ 255, 255, 255, 255 }; }      // White
        static ColorClassic Black() { return ColorClassic{ 0, 0, 0, 255 }; }            // Black
        static ColorClassic Magenta() { return ColorClassic{ 255, 0, 255, 255 }; }      // Magenta
        static ColorClassic Cyan() { return ColorClassic{ 0, 255, 255, 255 }; }         // Cyan
        static ColorClassic Transparent() { return ColorClassic{ 0, 0, 0, 0 }; }        // Transparent (no color)
    };

}  // namespace ClassicLauncher

#endif  // COLOR_H