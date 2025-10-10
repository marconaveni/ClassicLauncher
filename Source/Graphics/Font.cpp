#include "Font.h"

namespace ray
{
#include "raylib.h"
} // namespace ray

namespace ClassicLauncher
{


    struct FontImpl
    {
        ray::Font nativeFont{};
        bool owned{false};

        ~FontImpl()
        {
            if (owned && ray::IsFontValid(nativeFont))
            {
                ray::UnloadFont(nativeFont);
            }
        }
    };


    Font::Font()
        : m_pimpl(std::make_unique<FontImpl>())
    {
    }

    Font::~Font() = default;

    // Construtor e atribuição de movimento
    Font::Font(Font&& other) noexcept = default;
    Font& Font::operator=(Font&& other) noexcept = default;

    bool Font::IsValid() const
    {
        return m_pimpl && ray::IsFontValid(m_pimpl->nativeFont);
    }

    void Font::LoadFromFile(const std::filesystem::path& fileName, int fontSize, int* codepoints, int codepointCount)
    {
        m_pimpl->nativeFont = ray::LoadFontEx(fileName.string().c_str(), fontSize, codepoints, codepointCount);
        m_pimpl->owned = true; // Nós criamos, nós somos donos e devemos descarregá-la.

        SetSmooth(true);
    }

    Vector2f Font::MeasureTextEx(const std::string& text, float fontSize, float spacing)
    {
        if (!IsValid() || text.empty())
        {
            return {0.0f, 0.0f};
        }

        ray::Vector2 vec = ray::MeasureTextEx(m_pimpl->nativeFont, text.c_str(), fontSize, spacing);
        return {vec.x, vec.y};
    }

    int Font::GetBaseSize() const
    {
        return m_pimpl ? m_pimpl->nativeFont.baseSize : 0;
    }

    int Font::GetGlyphCount() const
    {
        return m_pimpl ? m_pimpl->nativeFont.glyphCount : 0;
    }

    int Font::GetGlyphPadding() const
    {
        return m_pimpl ? m_pimpl->nativeFont.glyphPadding : 0;
    }

    void Font::SetSmooth(bool status)
    {
        if (status == m_smooth)
        {
            return;
        }
        
        m_smooth = status;

        if (ray::IsFontValid(m_pimpl->nativeFont))
        {
            ray::SetTextureFilter(m_pimpl->nativeFont.texture, m_smooth ? ray::TEXTURE_FILTER_BILINEAR : ray::TEXTURE_FILTER_POINT);
        }
    }

    void* Font::GetNativeFont() const
    {
        return m_pimpl ? &m_pimpl->nativeFont : nullptr;
    }

} // namespace ClassicLauncher
