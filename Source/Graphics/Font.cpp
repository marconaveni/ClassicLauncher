#include "Font.h"

namespace ray
{
#include "raylib.h"
}  // namespace ray

namespace ClassicLauncher
{
    bool Font::IsValid() const
    {
        if (!_native)
        {
            return false;
        }
        return ray::IsFontValid(*static_cast<ray::Font*>(_native));
    }

    void Font::Unload()
    {
        if (!_native)
        {
            return;
        }
        if (_owned)
        {
            ray::UnloadFont(*static_cast<ray::Font*>(_native));
            delete static_cast<ray::Font*>(_native);  // delete own ptr caution here
        }
    }

    void Font::LoadFromFile(const std::filesystem::path& fileName, int fontSize, int* codepoints, int codepointCount)
    {
        ray::Font* rayFont = new ray::Font(ray::LoadFontEx(fileName.string().c_str(), fontSize, codepoints, codepointCount));
        baseSize = rayFont->baseSize;
        glyphCount = rayFont->glyphCount;
        glyphPadding = rayFont->glyphPadding;
        _native = rayFont;  // we keep the pointer here
        _owned = true;      // we allocate, then we unload later
    }

    Font Font::GetFontDefault()
    {
        static Font defaultFont;
        if (!defaultFont.IsValid())
        {
            static ray::Font s_default_native = ray::GetFontDefault();
            defaultFont.baseSize = s_default_native.baseSize;
            defaultFont.glyphCount = s_default_native.glyphCount;
            defaultFont.glyphPadding = s_default_native.glyphPadding;
            defaultFont._native = &s_default_native;  // ponteiro estável para a estática
            defaultFont._owned = false;               // NÃO descarregar: é o default
        }
        return defaultFont;
    }

    Vector2f Font::MeasureTextEx(const std::string& text, float fontSize, float spacing)
    {
        if (_native == nullptr || text.empty())
        {
            return ClassicLauncher::Vector2f{ 0, 0 };
        }

        ray::Vector2 vec = ray::MeasureTextEx(*static_cast<ray::Font*>(_native), text.c_str(), fontSize, spacing);
        return Vector2f{ vec.x, vec.y };
    }

}  // namespace ClassicLauncher
