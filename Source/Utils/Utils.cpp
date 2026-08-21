#include "Utils.h"

#include <cerrno>
#include <cstdint>
#include <cstdlib>

#include "ClassicAssert.h"
#include "Graphics/Image.h"
#include "Log.h"

namespace ClassicLauncher
{
    Color Utils::HexToColor(const std::string& hexColor)
    {

        std::string hexNewColor = hexColor;
        if (hexColor.size() < 8)
        {
            switch (hexColor.size())
            {
                case 2: hexNewColor += "0000FF"; break;
                case 4: hexNewColor += "00FF"; break;
                case 6: hexNewColor += "FF"; break;
                default:
                    LOG(LogError, "Invalid color value in .ini");
                    return Color::White;
                    break;
            }
        }

        std::uint32_t rgbaValue = 0;
        char* endPtr;
        errno = 0;
        unsigned long tempVal = std::strtoul(hexNewColor.c_str(), &endPtr, 16);

        if (*endPtr != '\0')
        {
            LOG(LogError, "Invalid color value in .ini. Character %c is not hexadecimal.", *endPtr);
            return Color::White;
        }
        else if (errno == ERANGE || tempVal > 0xFFFFFFFF)
        {
            LOG(LogError, "Color value out of range (overflow)");
            return Color::White;
        }

        rgbaValue = static_cast<std::uint32_t>(tempVal);
        return Color::FromInt(rgbaValue);
    }


    void Utils::SetSizeWithProportionFit(Vector2f& vector, const int widthResize, const int heightResize)
    {
        // Define the new desired width and height.
        float newWidth = static_cast<float>(widthResize);   // desired width
        float newHeight = static_cast<float>(heightResize); // desired height
        const float aspectRatio = vector.x / vector.y;      // Calculates the aspect ratio of the original image.

        if (newWidth / aspectRatio > newHeight) // Adjust the dimensions to maintain the proportions.
        {
            newWidth = newHeight * aspectRatio;
        }
        else
        {
            newHeight = newWidth / aspectRatio;
        }
        vector = Vector2f{newWidth, newHeight};
    }

    void Utils::SetSizeWithProportionFill(Vector2f& vector, const int widthResize, const int heightResize)
    {

        if (vector.x == 0.0f || vector.y == 0.0f) // Prevent division by zero when calculating the aspect ratio.
        {
            return;
        }

        float newWidth = static_cast<float>(widthResize);
        float newHeight = static_cast<float>(heightResize);

        const float aspectRatio = vector.x / vector.y; // Original aspect ratio (width / height).

        if (newWidth / aspectRatio < newHeight) // Ensure the resized area is completely filled while preserving aspect ratio.
        {
            newWidth = newHeight * aspectRatio; // Scale based on height.
        }
        else
        {
            newHeight = newWidth / aspectRatio; // Scale based on width.
        }
        vector = Vector2f{newWidth, newHeight};
    }

    void Utils::SetSizeWithProportion(Vector2f& vector, const int widthResize, const int heightResize, bool fill)
    {
        if (!fill)
        {
            SetSizeWithProportionFit(vector, widthResize, heightResize);
        }
        else
        {
            SetSizeWithProportionFill(vector, widthResize, heightResize);
        }
    }

    void Utils::ImageResize(Image& image, const int newWidth, const int newHeight)
    {
        Vector2f newSize{static_cast<float>(image.width), static_cast<float>(image.height)};
        SetSizeWithProportion(newSize, newWidth, newHeight);
        image.Resize(static_cast<int>(newSize.x), static_cast<int>(newSize.y));
    }

    void Utils::ImageResizeNN(Image& image, const int newWidth, const int newHeight)
    {
        Vector2f newSize{static_cast<float>(image.width), static_cast<float>(image.height)};
        SetSizeWithProportion(newSize, newWidth, newHeight);
        image.ResizeNN(static_cast<int>(newSize.x), static_cast<int>(newSize.y));
    }


    int Utils::SetIndexArray(const int index, const int maxArrayLength)
    {
        if (index >= maxArrayLength)
        {
            return std::abs(maxArrayLength - index);
        }
        if (index < 0)
        {
            return maxArrayLength - std::abs(index);
        }
        return index;
    }

    int Utils::CountChars(const std::string& text, const char compareChar)
    {
        int count = 0;
        for (const char c : text)
        {
            if (c == compareChar)
            {
                count++;
            }
        }
        return count;
    }


} // namespace ClassicLauncher