#include "Image.h"

namespace ray
{
#include "raylib.h"
}  // namespace ray

namespace ClassicLauncher
{

    void SetImage(Image& image, void* data, int width, int height, int mipmaps, int format)
    {
        image.m_data = data;
        image.m_width = width;
        image.m_height = height;
        image.m_mipmaps = mipmaps;
        image.m_format = format;
    }

    Image::Image(const std::filesystem::path& fileName)
    {
        LoadFromFile(fileName);
    }

    Image::Image(void* data, int width, int height, int mipmaps, int format)
        : m_data(data), m_width(width), m_height(height), m_mipmaps(mipmaps), m_format(format)
    {
    }

    Image::~Image()
    {
        Unload();
    }

    bool Image::LoadFromFile(const std::filesystem::path& fileName)
    {
        ray::Image img = ray::LoadImage(fileName.string().c_str());
        SetImage(*this, img.data, img.width, img.height, img.mipmaps, img.format);
        return false;
    }

    void Image::Resize(int newWidth, int newHeight)
    {
        ray::Image img{ m_data, m_width, m_height, m_mipmaps, m_format };
        ray::ImageResize(&img, newWidth, newHeight);
        SetImage(*this, img.data, img.width, img.height, img.mipmaps, img.format);
    }

    void Image::ResizeNN(int newWidth, int newHeight)
    {
        ray::Image img{ m_data, m_width, m_height, m_mipmaps, m_format };
        ray::ImageResizeNN(&img, newWidth, newHeight);
        SetImage(*this, img.data, img.width, img.height, img.mipmaps, img.format);
    }

    void Image::ImageCrop(RectInt crop)
    {
        ray::Image img{ m_data, m_width, m_height, m_mipmaps, m_format };
        ray::ImageCrop(&img, { (float)crop.x, (float)crop.x, (float)crop.width, (float)crop.height });
        SetImage(*this, img.data, img.width, img.height, img.mipmaps, img.format);
    }

    void Image::Unload()
    {
        free(m_data);
        m_data = nullptr;
    }

}  // namespace ClassicLauncher
