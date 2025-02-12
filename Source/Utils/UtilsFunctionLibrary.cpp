#include "UtilsFunctionLibrary.h"

#include <stdlib.h>
#include "Math.h"
#include "StringFunctionLibrary.h"

namespace ClassicLauncher
{

    void UtilsFunctionLibrary::SetSizeWithProportionFit(Vector2& texture, const int widthResize, const int heightResize)
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
        texture = Vector2{ newWidth, newHeight };
    }

    void UtilsFunctionLibrary::SetSizeWithProportionFill(Vector2& texture, const int widthResize, const int heightResize)
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
        texture = Vector2{ newWidth, newHeight };
    }

    void UtilsFunctionLibrary::SetSizeWithProportion(Vector2& texture, const int widthResize, const int heightResize, bool bFill)
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
        Vector2 newSize{ static_cast<float>(image.width), static_cast<float>(image.height) };
        SetSizeWithProportion(newSize, newWidth, newHeight);
        ::ImageResize(&image, static_cast<int>(newSize.x), static_cast<int>(newSize.y));
    }

    void UtilsFunctionLibrary::ImageResizeNN(Image& image, const int newWidth, const int newHeight)
    {
        Vector2 newSize{ static_cast<float>(image.width), static_cast<float>(image.height) };
        SetSizeWithProportion(newSize, newWidth, newHeight);
        ::ImageResizeNN(&image, static_cast<int>(newSize.x), static_cast<int>(newSize.y));
    }

    Texture2D UtilsFunctionLibrary::LoadTexture(const std::string& path, int width, int height)
    {
        Image img = LoadImage(path.c_str());
        width = (width > 0) ? width : img.width;
        height = (height > 0) ? height : img.height;
        ::ImageResize(&img, width, height);
        const Texture2D texture = LoadTextureFromImage(img);
        UnloadImage(img);
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

    void UtilsFunctionLibrary::UnloadClearTexture(Texture2D& texture)
    {
        UnloadTexture(texture);
        texture = Texture2D();
    }

    std::string UtilsFunctionLibrary::GetWorkingDirectory()
    {
        std::string newDir = ::GetWorkingDirectory();
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
        return ::ChangeDirectory(path.c_str());
    }

    int UtilsFunctionLibrary::CountChars(const std::string& text, const char compareChar)
    {
        int count = 0;
        for (char c : text)
        {
            if (c == compareChar)
            {
                count++;
            }
        }
        return count;
    }

    // void UtilsFunctionLibrary::SetTransforms(Transform& transformRef, TransformSource& transformSourceRef)
    // {
    //     Transform transform = transformRef;
    //     TransformSource transformSource = transformSourceRef;
    //     transform = transform.Multiply(Themes::GetScaleTexture());
    //     transformSource = transformSource.Multiply(Themes::GetScaleTexture());
    //     const float x = transform.rootX + transform.x;
    //     const float y = transform.rootY + transform.y;
    //     const float width = transform.width;
    //     const float height = transform.height;
    //     const float sourceX = transformSource.sourceX;
    //     const float sourceY = transformSource.sourceY;
    //     const float scaleWidth = transformSource.scaleWidth > 0.0f ? transformSource.scaleWidth : width;
    //     const float scaleHeight = transformSource.scaleHeight > 0.0f ? transformSource.scaleHeight : height;

    //     const Vector2 scale = { (scaleWidth * transform.scaleX * transform.rootScaleX), (scaleHeight * transform.scaleY * transform.rootScaleY) };
        
    //     //transformSourceRef.mSource = Rectangle(sourceX, sourceY, width, height);
    //     //transformRef.mTransform = Rectangle(x, y, scale.x, scale.y);
    // }

}  // namespace ClassicLauncher