#include "Image.h"

namespace ray
{
#include "raylib.h"
}  // namespace ray

namespace ClassicLauncher
{

    // class Image
    // {
    // public:

    //     Image() = default;
    //     Image(const std::filesystem::path& fileName);
    //     Image(void* data, int width, int height, int mipmaps, int format);
    //     ~Image();
    //     Image(const Image&) = delete;
    //     Image& operator=(const Image&) = delete;

    //     bool LoadFromFile(const std::filesystem::path& fileName);
    //     void Resize(int newWidth, int newHeight);  // Resize image (Bicubic scaling algorithm)
    //     void ResizeNN(int newWidth,int newHeight); // Resize image (Nearest-Neighbor scaling algorithm)
    //     void ImageCrop(RectInt crop);
    //     void Unload();

    // private:

    //     friend class Texture;
    //     friend void SetImage(Image& image, void* data, int width, int height, int mipmaps, int format);

    //     void *m_data{};
    //     int m_width{};
    //     int m_height{};
    //     int m_mipmaps{};
    //     int m_format{};
    // };

    // void SetImage(Image& image, void* data, int width, int height, int mipmaps, int format)
    // {
    //     image.m_data = data;
    //     image.m_width = width;
    //     image.m_height = height;
    //     image.m_mipmaps = mipmaps;
    //     image.m_format = format;
    // }

    // Image::Image(const std::filesystem::path& fileName)
    // {
    //     LoadFromFile(fileName);
    // }

    // Image::Image(void* data, int width, int height, int mipmaps, int format)
    //     : m_data(data), m_width(width), m_height(height), m_mipmaps(mipmaps), m_format(format)
    // {
    // }

    // Image::~Image()
    // {
    //     Unload();
    // }

    // bool Image::LoadFromFile(const std::filesystem::path& fileName)
    // {
    //     ray::Image img = ray::LoadImage(fileName.string().c_str());
    //     SetImage(*this, img.data, img.width, img.height, img.mipmaps, img.format);
    //     return false;
    // }

    // void Image::Resize(int newWidth, int newHeight)
    // {
    //     ray::Image img{ m_data, m_width, m_height, m_mipmaps, m_format };
    //     ray::ImageResize(&img, newWidth, newHeight);
    //     SetImage(*this, img.data, img.width, img.height, img.mipmaps, img.format);
    // }

    // void Image::ResizeNN(int newWidth, int newHeight)
    // {
    //     ray::Image img{ m_data, m_width, m_height, m_mipmaps, m_format };
    //     ray::ImageResizeNN(&img, newWidth, newHeight);
    //     SetImage(*this, img.data, img.width, img.height, img.mipmaps, img.format);
    // }

    // void Image::ImageCrop(RectInt crop)
    // {
    //     ray::Image img{ m_data, m_width, m_height, m_mipmaps, m_format };
    //     ray::ImageCrop(&img, { (float)crop.x, (float)crop.x, (float)crop.width, (float)crop.height });
    //     SetImage(*this, img.data, img.width, img.height, img.mipmaps, img.format);
    // }

    // void Image::Unload()
    // {
    //     free(m_data);
    //     m_data = nullptr;
    // }

    Image::Image(void* data, int width, int height, int mipmaps, int format)
        : data(data), width(width), height(height), mipmaps(mipmaps), format(format)
    {
    }

    bool Image::IsValid() const
    {
        return ((data != NULL) &&  // Validate pixel data available
                (width > 0) &&     // Validate image width
                (height > 0) &&    // Validate image height
                (format > 0) &&    // Validate image format
                (mipmaps > 0));    // Validate image mipmaps (at least 1 for basic mipmap level);
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
        rayImg = ray::GenImageColor(widthImg, heightImg, { color.r, color.g, color.b, color.a });
        data = rayImg.data;
        width = rayImg.width;
        height = rayImg.height;
        mipmaps = rayImg.mipmaps;
        format = rayImg.format;
    }

}  // namespace ClassicLauncher
