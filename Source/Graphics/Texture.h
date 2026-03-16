#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>
#include <string>

#include "Data/Rectangle.h"

namespace ClassicLauncher
{

    enum PixelFormat
    {
        UncompressedGrayScale = 1, // 8 bit per pixel (no alpha)
        UncompressedGrayAlpha,     // 8*2 bpp (2 channels)
        UncompressedR5G6B5,        // 16 bpp
        UncompressedR8G8B8,        // 24 bpp
        UncompressedR5G5B5A1,      // 16 bpp (1 bit alpha)
        UncompressedR4G4B4A4,      // 16 bpp (4 bit alpha)
        UncompressedR8G8B8A8,      // 32 bpp
        UncompressedR32,           // 32 bpp (1 channel - float)
        UncompressedR32G32B32,     // 32*3 bpp (3 channels - float)
        UncompressedR32G32B32A32,  // 32*4 bpp (4 channels - float)
        UncompressedR16,           // 16 bpp (1 channel - half float)
        UncompressedR16G16B16,     // 16*3 bpp (3 channels - half float)
        UncompressedR16G16B16A16,  // 16*4 bpp (4 channels - half float)
        CompressedDXT1RGB,         // 4 bpp (no alpha)
        CompressedDXT1RGBA,        // 4 bpp (1 bit alpha)
        CompressedDXT3RGBA,        // 8 bpp
        CompressedDXT5RGBA,        // 8 bpp
        CompressedETC1RGB,         // 4 bpp
        CompressedETC2RGB,         // 4 bpp
        CompressedETC2EACRGBA,     // 8 bpp
        CompressedPVRTRGB,         // 4 bpp
        CompressedPVRTRGBA,        // 4 bpp
        CompressedASTC4X4RGBA,     // 8 bpp
        CompressedASTC8X8RGBA      // 2 bpp
    };

    class Image;
    class RenderTexture;

    class Texture
    {
    public:

        Texture() = default;
        Texture(const std::filesystem::path& fileName);
        Texture(int width, int height, int mipmaps, int format);
        ~Texture();
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        bool LoadFromFile(const std::filesystem::path& fileName);
        bool LoadFromImage(Image* image);
        bool LoadFromData(void* data, int width, int height, int mipmaps, int format);
        void SetSmooth(bool status);
        void SetWrap(int wrap);
        [[nodiscard]] bool IsValid() const;
        void Update(const void* pixels);
        void Update(const void* pixels, RectInt rec);
        void Unload();
        [[nodiscard]] bool IsSmooth() const { return m_smooth; }
        [[nodiscard]] unsigned int GetId() const { return m_id; }
        [[nodiscard]] int GetMipmaps() const { return m_mipmaps; }
        [[nodiscard]] int GetFormat() const { return m_format; }
        [[nodiscard]] Sizef GetSize() const { return Sizei(m_width, m_height).ToFloat(); }
        static unsigned int GetTextureSizeBytes();

    private:

        friend class RenderTexture;

        unsigned int m_id{0};
        int m_width{0};
        int m_height{0};
        int m_mipmaps{0};
        int m_format{0};
        bool m_smooth{false};
    };

} // namespace ClassicLauncher

#endif // TEXTURE
