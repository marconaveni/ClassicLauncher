#ifndef FONT_H
#define FONT_H

#include <filesystem>
#include <string>

#include "Data/Vector2.h"


namespace ClassicLauncher
{
    struct FontImpl;

    class Font
    {
    public:

        Font();
        ~Font();

        Font(Font&& other) noexcept;
        Font& operator=(Font&& other) noexcept;
        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        void LoadFromFile(const std::filesystem::path& fileName, int fontSize, int* codepoints, int codepointCount);
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] Vector2f MeasureTextEx(const std::string& text, float fontSize, float spacing);

        [[nodiscard]] int GetBaseSize() const;
        [[nodiscard]] int GetGlyphCount() const;
        [[nodiscard]] int GetGlyphPadding() const;

        void SetSmooth(bool status);

        [[nodiscard]] void* GetNativeFont() const;

    private:

        std::unique_ptr<FontImpl> m_pimpl;
        bool m_smooth{false};
    };


} // namespace ClassicLauncher

#endif // FONT_H