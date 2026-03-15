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

        void Unload()
        {
            if (owned && ray::IsFontValid(nativeFont))
            {
                owned = false;
                ray::UnloadFont(nativeFont);
                nativeFont = ray::Font();
            }
        }
        ~FontImpl() { Unload(); }
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
        m_fileName = fileName;
        m_fontSize = fontSize;
        m_codepoints = codepoints;
        m_codepointCount = codepointCount;
        Load();
    }

    void Font::Load()
    {
        m_pimpl->nativeFont = ray::LoadFontEx(m_fileName.string().c_str(), m_fontSize, m_codepoints, m_codepointCount);
        m_pimpl->owned = true; // We created it, we own it, and we must unload it.
        SetSmooth(true);
    }

    void Font::Unload()
    {
        m_pimpl->Unload();
        m_smooth = false;
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
