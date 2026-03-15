#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "Graphics/Font.h"


namespace ClassicLauncher
{

    class FontManager
    {
    public:

        FontManager() = default;
        ~FontManager();

        void Init();
        void Unload();
        std::string Load(const std::filesystem::path& path, int size);
        Font* GetFont(const std::string& name);
        void OnGraphicsRestore();
        void OnGraphicsLost();

        std::unordered_map<std::string, std::unique_ptr<Font>> m_font{};

    private:

        std::string GenerateFileName(const std::filesystem::path& path, int size);
    };

} // namespace ClassicLauncher


#endif // FONT_MANAGER_H