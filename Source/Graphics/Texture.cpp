#include "Texture.h"

#include "Graphics/Image.h"

namespace ray
{
#include "rlgl.h"
} // namespace ray


namespace ClassicLauncher
{
    static unsigned int s_textureSizeBytes = 0;

    static unsigned int GetBytesPerPixel(int format)
    {
        switch (format)
        {
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE: return 1;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA: return 2;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R5G6B5: return 2;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8: return 3;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1: return 2;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4: return 2;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8: return 4;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R32: return 4;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32: return 12;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32: return 16;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R16: return 2;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16: return 6;
            case ray::RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16: return 8;
            default: return 0;
        }
    }

    Texture::Texture(const std::filesystem::path& fileName)
    {
        LoadFromFile(fileName);
    }

    Texture::Texture(int width, int height, int mipmaps, int format)
        : m_width(width)
        , m_height(height)
        , m_mipmaps(mipmaps)
        , m_format(format)
    {
    }

    Texture::~Texture()
    {
        Unload();
    }

    bool Texture::LoadFromFile(const std::filesystem::path& fileName)
    {
        Image image;
        image.LoadFromFile(fileName);
        return LoadFromImage(&image);
    }

    bool Texture::LoadFromImage(Image* image)
    {
        Unload();

        if ((image->width == 0) && (image->height == 0))
        {
            return false;
        }

        m_id = ray::rlLoadTexture(image->data, image->width, image->height, image->format, image->mipmaps);

        m_width = image->width;
        m_height = image->height;
        m_mipmaps = image->mipmaps;
        m_format = image->format;

        const int size = GetBytesPerPixel(m_format);
        s_textureSizeBytes += m_width * m_height * size;

        return IsValid();
    }

    bool Texture::LoadFromData(void* data, int width, int height, int mipmaps, int format)
    {
        Unload();
        m_width = width;
        m_height = height;
        m_mipmaps = mipmaps;
        m_format = format;
        m_id = ray::rlLoadTexture(data, width, height, format, mipmaps);

        s_textureSizeBytes += m_width * m_height * 4;
        return true;
    }

    void Texture::SetSmooth(bool status)
    {
        if (status == m_smooth)
        {
            return;
        }

        m_smooth = status;

        if (!IsValid())
        {
            return;
        }

        ray::rlTextureParameters(m_id, RL_TEXTURE_MIN_FILTER, (status) ? RL_TEXTURE_FILTER_LINEAR : RL_TEXTURE_FILTER_NEAREST);
        ray::rlTextureParameters(m_id, RL_TEXTURE_MAG_FILTER, (status) ? RL_TEXTURE_FILTER_LINEAR : RL_TEXTURE_FILTER_NEAREST);
    }

    void Texture::SetWrap(int wrap)
    {
    }

    bool Texture::IsValid() const
    {
        return ((m_id > 0) &&     // Validate OpenGL id (texture uploaded to GPU)
                (m_width > 0) &&  // Validate texture width
                (m_height > 0) && // Validate texture height
                (m_format > 0) && // Validate texture pixel format
                (m_mipmaps > 0)); // Validate texture mipmaps (at least 1 for basic mipmap level)
    }

    void Texture::Update(const void* pixels)
    {
        ray::rlUpdateTexture(m_id, 0, 0, m_width, m_height, m_format, pixels);
    }

    void Texture::Update(const void* pixels, RectInt rec)
    {
        ray::rlUpdateTexture(m_id, rec.x, rec.y, rec.width, rec.height, m_format, pixels);
    }

    void Texture::Unload()
    {
        if (m_id != 0)
        {
            const int size = GetBytesPerPixel(m_format);
            s_textureSizeBytes -= m_width * m_height * size;

            ray::rlUnloadTexture(m_id);
            m_id = 0;
            m_width = 0;
            m_height = 0;
            m_format = 0;
            m_mipmaps = 0;
        }
    }

    unsigned int Texture::GetTextureSizeBytes()
    {
        return s_textureSizeBytes;
    }

} // namespace ClassicLauncher