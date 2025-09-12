#ifndef IMAGE
#define IMAGE

#include <filesystem>
#include "Data/Rectangle.h"
#include "Data/Color.h"


namespace ClassicLauncher
{

    class Image
    {
    public:

        Image() = default;
        ~Image() = default; 
        Image(void* data, int width, int height, int mipmaps, int format);
        void* data{};
        int width{};
        int height{};
        int mipmaps{};
        int format{};

        [[nodiscard]] bool IsValid() const;
        void LoadFromFile(const std::filesystem::path& fileName);
        void Unload();
        void Resize(int newWidth, int newHeight);
        void ResizeNN(int newWidth, int newHeight);
        void CopyTo(Image& otherImage);
        void GenerateColor(int widthImg, int heightImg, Color color);
    };

}  // namespace ClassicLauncher

#endif  // TEXTURE