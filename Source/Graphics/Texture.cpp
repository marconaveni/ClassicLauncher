#include "Texture.h"

#include "Graphics/Image.h"

namespace ray
{
#include "rlgl.h"
} // namespace ray


namespace ClassicLauncher
{
    Texture::Texture(const std::filesystem::path& fileName)
    {
        LoadFromFile(fileName);
    }

    Texture::Texture(int width, int height, int mipmaps, int format)
        : m_width(width), m_height(height), m_mipmaps(mipmaps), m_format(format)
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

        ray::rlTextureParameters(m_id,
                                 RL_TEXTURE_MIN_FILTER,
                                 (status) ? RL_TEXTURE_FILTER_LINEAR : RL_TEXTURE_FILTER_NEAREST);
        ray::rlTextureParameters(m_id,
                                 RL_TEXTURE_MAG_FILTER,
                                 (status) ? RL_TEXTURE_FILTER_LINEAR : RL_TEXTURE_FILTER_NEAREST);
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
            ray::rlUnloadTexture(m_id);
            m_id = 0;
            m_width = 0;
            m_height = 0;
            m_format = 0;
            m_mipmaps = 0;
        }
    }

} // namespace ClassicLauncher