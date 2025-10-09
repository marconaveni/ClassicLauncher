#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <unordered_map>
#include <string>
#include <filesystem>
#include "Graphics/Font.h"
#include <memory>


namespace ClassicLauncher
{


    class FontManager
    {
    public:

        FontManager();
        ~FontManager();

        void Init();
        std::string Load(const std::filesystem::path& path, int size);
        Font* GetFont(const std::string& name);
        
        std::unordered_map<std::string, std::unique_ptr<Font>> m_font;

    private:

        std::string GenerateFileName(const std::filesystem::path& path, int size);

    };

   
} // namespace ClassicLauncher


#endif // FONT_MANAGER_H