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
                    LOG(LOG_CLASSIC_ERROR, "Invalid color value in .ini");
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
            LOG(LOG_CLASSIC_ERROR, "Invalid color value in .ini. Character %c is not hexadecimal.", *endPtr);
            return Color::White;
        }
        else if (errno == ERANGE || tempVal > 0xFFFFFFFF)
        {
            LOG(LOG_CLASSIC_ERROR, "Color value out of range (overflow)");
            return Color::White;
        }

        rgbaValue = static_cast<std::uint32_t>(tempVal);
        return Color::FromInt(rgbaValue);
    }


    void Utils::SetSizeWithProportionFit(Vector2f& vector, const int widthResize, const int heightResize)
    {
        // Define a nova largura e altura desejadas
        float newWidth = static_cast<float>(widthResize);   // Largura desejada
        float newHeight = static_cast<float>(heightResize); // Altura desejada
        const float aspectRatio = vector.x / vector.y;      // Calcula a proporção da imagem original

        if (newWidth / aspectRatio > newHeight) // Ajusta as dimensões para manter a proporção
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

        if (vector.x == 0.0f || vector.y == 0.0f) // check evita cair numa divisão por 0
        {
            return;
        }

        float newWidth = static_cast<float>(widthResize);
        float newHeight = static_cast<float>(heightResize);
        const float aspectRatio = vector.x / vector.y;

        if (newWidth / aspectRatio < newHeight) // Ajusta para preencher completamente a área desejada (Fill)
        {
            newWidth = newHeight * aspectRatio; // Se a altura está pequena demais, ajusta a largura primeiro
        }
        else
        {
            newHeight = newWidth / aspectRatio; // Se a largura está pequena demais, ajusta a altura primeiro
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