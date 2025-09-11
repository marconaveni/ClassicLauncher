#ifndef IMAGE
#define IMAGE

#include <filesystem>
#include "Data/Rectangle.h"

struct Image;

namespace ClassicLauncher
{



    class Image
    {
    public:

        Image() = default;
        Image(const std::filesystem::path& fileName);
        Image(void* data, int width, int height, int mipmaps, int format);
        ~Image();
        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        bool LoadFromFile(const std::filesystem::path& fileName);
        void Resize(int newWidth, int newHeight);  // Resize image (Bicubic scaling algorithm)
        void ResizeNN(int newWidth,int newHeight); // Resize image (Nearest-Neighbor scaling algorithm)
        void ImageCrop(RectInt crop);   
        void Unload();

    private:

        friend class Texture;
        friend void SetImage(Image& image, void* data, int width, int height, int mipmaps, int format);

        void *m_data{};
        int m_width{};
        int m_height{};
        int m_mipmaps{};  
        int m_format{};  
    };

}  // namespace ClassicLauncher

#endif  // TEXTURE