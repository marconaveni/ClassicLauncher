#include "Image.h"

namespace ray
{
#include "raylib.h"
} // namespace ray

namespace ClassicLauncher
{

    Image::Image(void* data, int width, int height, int mipmaps, int format)
        : data(data), width(width), height(height), mipmaps(mipmaps), format(format)
    {
    }

    bool Image::IsValid() const
    {
        return ((data != NULL) && // Validate pixel data available
                (width > 0) &&    // Validate image width
                (height > 0) &&   // Validate image height
                (format > 0) &&   // Validate image format
                (mipmaps > 0));   // Validate image mipmaps (at least 1 for basic mipmap level);
    }

    void Image::LoadFromFile(const std::filesystem::path& fileName)
    {
        ray::Image rayImg{};
        rayImg = ray::LoadImage(fileName.string().c_str());
        data = rayImg.data;
        width = rayImg.width;
        height = rayImg.height;
        mipmaps = rayImg.mipmaps;
        format = rayImg.format;
    }

    void Image::Unload()
    {
        if (data != nullptr)
        {
            free(data);
            data = nullptr;
        }
    }

    void Image::Resize(int newWidth, int newHeight)
    {
        ray::Image rayImg{};
        rayImg.data = data;
        rayImg.width = width;
        rayImg.height = height;
        rayImg.mipmaps = mipmaps;
        rayImg.format = format;
        ray::ImageResize(&rayImg, newWidth, newHeight);
        data = rayImg.data;
        width = rayImg.width;
        height = rayImg.height;
        mipmaps = rayImg.mipmaps;
        format = rayImg.format;
    }

    void Image::ResizeNN(int newWidth, int newHeight)
    {
        ray::Image rayImg{};
        rayImg.data = data;
        rayImg.width = width;
        rayImg.height = height;
        rayImg.mipmaps = mipmaps;
        rayImg.format = format;
        ray::ImageResizeNN(&rayImg, newWidth, newHeight);
        data = rayImg.data;
        width = rayImg.width;
        height = rayImg.height;
        mipmaps = rayImg.mipmaps;
        format = rayImg.format;
    }

    void Image::CopyTo(Image& otherImage)
    {
        ray::Image rayImg{};
        rayImg.data = data;
        rayImg.width = width;
        rayImg.height = height;
        rayImg.mipmaps = mipmaps;
        rayImg.format = format;
        ray::Image rayCopiedImg{};
        rayCopiedImg = ray::ImageCopy(rayImg);
        otherImage.data = rayCopiedImg.data;
        otherImage.width = rayCopiedImg.width;
        otherImage.height = rayCopiedImg.height;
        otherImage.mipmaps = rayCopiedImg.mipmaps;
        otherImage.format = rayCopiedImg.format;
    }

    void Image::GenerateColor(int widthImg, int heightImg, Color color)
    {
        ray::Image rayImg{};
        rayImg = ray::GenImageColor(widthImg, heightImg, {color.r, color.g, color.b, color.a});
        data = rayImg.data;
        width = rayImg.width;
        height = rayImg.height;
        mipmaps = rayImg.mipmaps;
        format = rayImg.format;
    }

} // namespace ClassicLauncher
