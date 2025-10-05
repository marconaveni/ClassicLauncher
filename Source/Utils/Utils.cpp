#include "Utils.h"

//#include <cstdlib>

#include "Graphics/Image.h"
#include "String.h"
#include "ClassicAssert.h"

namespace ClassicLauncher
{

    void Utils::SetSizeWithProportionFit(Vector2f& vector, const int widthResize, const int heightResize)
    {
        // Define a nova largura e altura desejadas
        float newWidth = static_cast<float>(widthResize);   // Largura desejada
        float newHeight = static_cast<float>(heightResize); // Altura desejada
        const float aspectRatio = vector.x / vector.y;    // Calcula a proporção da imagem original

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

    void Utils::SetSizeWithProportion(Vector2f& vector, const int widthResize, const int heightResize, bool bFill)
    {
        if (!bFill)
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

    // std::string Utils::GetWorkingDirectory()
    // {
    //     std::string newDir = rlw::GetWorkingDirectory();
    //     newDir += "/";
    //     return String::NormalizePath(newDir);
    // }

//     std::string Utils::GetHomeDir()
//     {
// #ifdef _WIN32
//         const char* homeDir = "USERPROFILE";
// #else
//         const char* homeDir = "HOME";
// #endif
//         std::string env = getenv(homeDir);
//         env += "/";
//         return String::NormalizePath(env);
//     }

    // bool Utils::ChangeDirectory(const std::string& path)
    // {
    //     return rlw::ChangeDirectory(path.c_str());
    // }

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