#ifndef IMAGE_H
#define IMAGE_H

#include <filesystem>

#include "Data/Color.h"
#include "Data/Rectangle.h"


namespace ClassicLauncher
{

    class Image
    {
    public:

        Image() = default;
        ~Image() = default;
        Image(void* data, int width, int height, int mipmaps, int format);
        void* data{nullptr};
        int width{0};
        int height{0};
        int mipmaps{0};
        int format{0};

        [[nodiscard]] bool IsValid() const;
        void LoadFromFile(const std::filesystem::path& fileName);
        void Unload();
        void Resize(int newWidth, int newHeight);
        void ResizeNN(int newWidth, int newHeight);
        void CopyTo(Image& otherImage);
        void GenerateColor(int widthImg, int heightImg, Color color);
    };

} // namespace ClassicLauncher

#endif // TEXTURE