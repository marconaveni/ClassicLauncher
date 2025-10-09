#include "FontManager.h"
#include "Graphics/Font.h"


namespace ClassicLauncher
{


    FontManager::FontManager()
    {
    }

    FontManager::~FontManager()
    {
        m_font.clear();
    }

    void FontManager::Init()
    {
    }

    std::string FontManager::Load(const std::filesystem::path& path, int size)
    {
        std::string name = GenerateFileName(path, size);
        m_font[name] = std::make_unique<Font>();
        m_font[name]->LoadFromFile(path, size, nullptr, 250);
        return name;
    }

    Font* FontManager::GetFont(const std::string& name)
    {
        auto it = m_font.find(name);
        if (it == m_font.end())
        {     
            return nullptr;
        }
        return m_font[name].get();
    }

    std::string FontManager::GenerateFileName(const std::filesystem::path& path, int size)
    {
        std::string name;
        name.append(path.filename().string());
        name.append("_");
        name.append(std::to_string(size));
        return name;
    }


} // namespace ClassicLauncher
