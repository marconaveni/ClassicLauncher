#ifndef FONT
#define FONT

#include <filesystem>
#include <string>
#include "Data/Vector2.h"


namespace ClassicLauncher
{

    class Font
    {
    public:
        int baseSize{};
        int glyphCount{};
        int glyphPadding{};
  
        void* _native{};       // ponteiro pro nativo (::Font*)
        bool _owned{ false };  // true: veio de LoadFontEx (descarrega); false: default font (não descarrega)

        [[nodiscard]] bool IsValid() const;
        void Unload();
        void LoadFromFile(const std::filesystem::path& fileName, int fontSize, int* codepoints, int codepointCount);
        static Font GetFontDefault();
        [[nodiscard]] Vector2f MeasureTextEx(const std::string& text, float fontSize, float spacing);
    };


}  // namespace ClassicLauncher

#endif  // FONT