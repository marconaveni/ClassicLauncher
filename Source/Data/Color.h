#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

namespace ClassicLauncher
{
    class Color
    {
    public:

        unsigned char r{255};
        unsigned char g{255};
        unsigned char b{255};
        unsigned char a{255};

        constexpr Color() = default;

        constexpr explicit Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
            : r(red), g(green), b(blue), a(alpha)
        {
        }

        constexpr bool operator==(const Color& other) const
        {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }
        constexpr bool operator!=(const Color& other) const { return !(*this == other); }

        constexpr void SetRed(int color, bool clamp = true) { r = (clamp) ? Clamp(color) : color; }
        constexpr void SetGreen(int color, bool clamp = true) { g = (clamp) ? Clamp(color) : color; }
        constexpr void SetBlue(int color, bool clamp = true) { b = (clamp) ? Clamp(color) : color; }
        constexpr void SetOpacity(int alpha, bool clamp = true) { a = (clamp) ? Clamp(alpha) : alpha; }

        constexpr std::uint32_t ToInteger() const
        {
            return static_cast<std::uint32_t>((r << 24) | (g << 16) | (b << 8) | a);
        }
        static Color FromInt(std::uint32_t rgba)
        {
            return Color((rgba >> 24) & 0xFF, (rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF, rgba & 0xFF);
        }

        static Color WhiteGray;   // Gray (Almost White)
        static Color LightGray;   // Light Gray
        static Color Gray;        // Gray
        static Color DarkGray;    // Dark Gray
        static Color Yellow;      // Yellow
        static Color Gold;        // Gold
        static Color Orange;      // Orange
        static Color Pink;        // Pink
        static Color Red;         // Red
        static Color Maroon;      // Maroon
        static Color Green;       // Green
        static Color Lime;        // Lime
        static Color DarkGreen;   // Dark Green
        static Color SkyBlue;     // Sky Blue
        static Color Blue;        // Blue
        static Color DarkBlue;    // Dark Blue
        static Color Purple;      // Purple
        static Color Violet;      // Violet
        static Color DarkPurple;  // Dark Purple
        static Color Beige;       // Beige
        static Color Brown;       // Brown
        static Color DarkBrown;   // DarkBrown
        static Color White;       // White
        static Color Magenta;     // Magenta
        static Color Cyan;        // Cyan
        static Color Black;       // Black
        static Color Transparent; // Transparent (no color)

    private:

        constexpr unsigned char Clamp(int value)
        {
            const int clamp = (value >= 0 && value <= 255) ? value : (value < 0) ? 0 : 255;
            return static_cast<unsigned char>(clamp);
        }
    };

    inline Color Color::WhiteGray(245, 245, 245); // Gray (Almost White)
    inline Color Color::LightGray(200, 200, 200); // Light Gray
    inline Color Color::Gray(130, 130, 130);      // Gray
    inline Color Color::DarkGray(80, 80, 80);     // Dark Gray
    inline Color Color::Yellow(253, 249, 0);      // Yellow
    inline Color Color::Gold(255, 203, 0);        // Gold
    inline Color Color::Orange(255, 161, 0);      // Orange
    inline Color Color::Pink(255, 109, 194);      // Pink
    inline Color Color::Red(230, 41, 55);         // Red
    inline Color Color::Maroon(190, 33, 55);      // Maroon
    inline Color Color::Green(0, 228, 48);        // Green
    inline Color Color::Lime(0, 158, 47);         // Lime
    inline Color Color::DarkGreen(0, 117, 44);    // Dark Green
    inline Color Color::SkyBlue(102, 191, 255);   // Sky Blue
    inline Color Color::Blue(0, 121, 241, 255);   // Blue
    inline Color Color::DarkBlue(0, 82, 172);     // Dark Blue
    inline Color Color::Purple(200, 122, 255);    // Purple
    inline Color Color::Violet(135, 60, 190);     // Violet
    inline Color Color::DarkPurple(112, 31, 126); // Dark Purple
    inline Color Color::Beige(211, 176, 131);     // Beige
    inline Color Color::Brown(127, 106, 79);      // Brown
    inline Color Color::DarkBrown(76, 63, 47);    // DarkBrown
    inline Color Color::White(255, 255, 255);     // White
    inline Color Color::Magenta(255, 0, 255);     // Magenta
    inline Color Color::Cyan(0, 255, 255);        // Cyan
    inline Color Color::Black(0, 0, 0, 255);      // Black
    inline Color Color::Transparent(0, 0, 0, 0);  // Transparent (no color)

} // namespace ClassicLauncher

#endif // COLOR_H