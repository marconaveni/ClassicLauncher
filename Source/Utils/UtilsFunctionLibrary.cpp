#include "UtilsFunctionLibrary.h"

#include <cstdlib>
#include "StringFunctionLibrary.h"
#include "Graphics/Image.h"

namespace ClassicLauncher
{

    void UtilsFunctionLibrary::SetSizeWithProportionFit(Vector2f& texture, const int widthResize, const int heightResize)
    {
        // Define a nova largura e altura desejadas
        float newWidth = static_cast<float>(widthResize);    // Largura desejada
        float newHeight = static_cast<float>(heightResize);  // Altura desejada
        const float aspectRatio = texture.x / texture.y;     // Calcula a proporção da imagem original

        if (newWidth / aspectRatio > newHeight)  // Ajusta as dimensões para manter a proporção
        {
            newWidth = newHeight * aspectRatio;
        }
        else
        {
            newHeight = newWidth / aspectRatio;
        }
        texture = Vector2f{ newWidth, newHeight };
    }

    void UtilsFunctionLibrary::SetSizeWithProportionFill(Vector2f& texture, const int widthResize, const int heightResize)
    {
        float newWidth = static_cast<float>(widthResize);
        float newHeight = static_cast<float>(heightResize);
        const float aspectRatio = texture.x / texture.y;

        if (newWidth / aspectRatio < newHeight)  // Ajusta para preencher completamente a área desejada (Fill)
        {
            newWidth = newHeight * aspectRatio;  // Se a altura está pequena demais, ajusta a largura primeiro
        }
        else
        {
            newHeight = newWidth / aspectRatio;  // Se a largura está pequena demais, ajusta a altura primeiro
        }
        texture = Vector2f{ newWidth, newHeight };
    }

    void UtilsFunctionLibrary::SetSizeWithProportion(Vector2f& texture, const int widthResize, const int heightResize, bool bFill)
    {
        if (!bFill)
        {
            SetSizeWithProportionFit(texture, widthResize, heightResize);
        }
        else
        {
            SetSizeWithProportionFill(texture, widthResize, heightResize);
        }
    }

    void UtilsFunctionLibrary::ImageResize(Image& image, const int newWidth, const int newHeight)
    {
        Vector2f newSize{ static_cast<float>(image.width), static_cast<float>(image.height) };
        SetSizeWithProportion(newSize, newWidth, newHeight);
        // rlw::ImageResize(&image, static_cast<int>(newSize.x), static_cast<int>(newSize.y));
        image.Resize( static_cast<int>(newSize.x), static_cast<int>(newSize.y));
    }

    void UtilsFunctionLibrary::ImageResizeNN(Image& image, const int newWidth, const int newHeight)
    {
        Vector2f newSize{ static_cast<float>(image.width), static_cast<float>(image.height) };
        SetSizeWithProportion(newSize, newWidth, newHeight);
        // rlw::ImageResizeNN(&image, static_cast<int>(newSize.x), static_cast<int>(newSize.y));
        image.ResizeNN( static_cast<int>(newSize.x), static_cast<int>(newSize.y));
    }

    rlw::Texture2D UtilsFunctionLibrary::LoadTexture(const std::string& path, int width, int height)
    {
        Image image;
        image.LoadFromFile(path);
        width = (width > 0) ? width : image.width;
        height = (height > 0) ? height : image.height;
        image.Resize( width, height);
        const rlw::Texture2D texture = rlw::LoadTextureFromImage(image);
        image.Unload();
        return texture;
    }

    int UtilsFunctionLibrary::SetIndexArray(const int index, const int maxArrayLength)
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

    void UtilsFunctionLibrary::UnloadClearTexture(rlw::Texture2D& texture)
    {
        rlw::UnloadTexture(texture);
        texture = rlw::Texture2D();
    }

    std::string UtilsFunctionLibrary::GetWorkingDirectory()
    {
        std::string newDir = rlw::GetWorkingDirectory();
        newDir += "/";
        return StringFunctionLibrary::NormalizePath(newDir);
    }

    std::string UtilsFunctionLibrary::GetHomeDir()
    {
#ifdef _WIN32
        const char* homeDir = "USERPROFILE";
#else
        const char* homeDir = "HOME";
#endif
        std::string env = getenv(homeDir);
        env += "/";
        return StringFunctionLibrary::NormalizePath(env);
    }

    bool UtilsFunctionLibrary::ChangeDirectory(const std::string& path)
    {
        return rlw::ChangeDirectory(path.c_str());
    }

    int UtilsFunctionLibrary::CountChars(const std::string& text, const char compareChar)
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



}  // namespace ClassicLauncher