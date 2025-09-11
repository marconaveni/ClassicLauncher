#ifndef TEXTURE
#define TEXTURE

#include <string>
#include <filesystem>
#include "Data/Rectangle.h"

namespace ClassicLauncher
{

    class Image;

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
        [[nodiscard]] unsigned int GetId() const { return m_id; }
        [[nodiscard]] int GetMipmaps() const { return m_mipmaps; }
        [[nodiscard]] int GetFormat() const { return m_format; }
        [[nodiscard]] Vector2i GetSize() const { return {m_width, m_height}; }

    private:

        friend class RenderTexture;

        unsigned int m_id{};
        int m_width{};
        int m_height{};
        int m_mipmaps{};  
        int m_format{};   
    };

}  // namespace ClassicLauncher

#endif  // TEXTURE

