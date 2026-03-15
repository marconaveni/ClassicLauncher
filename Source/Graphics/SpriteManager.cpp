#include "SpriteManager.h"

#include "Data/Color.h"
#include "Themes/ThemesManager.h"

namespace ClassicLauncher
{
    SpriteManager::SpriteManager()
    {
    }

    void SpriteManager::Init()
    {
        Image transparentImage;
        transparentImage.GenerateColor(1, 1, Color::Transparent);
        Image blackImage;
        blackImage.GenerateColor(1, 1, Color::Black);
        LoadSprite("transparent", transparentImage);
        LoadSprite("black", blackImage);
        transparentImage.Unload();
        blackImage.Unload();
    }

    void SpriteManager::LoadRenderTexture(const std::string& name, int width, int height)
    {
        auto it = m_renderTextureMap.find(name);
        if (it == m_renderTextureMap.end())
        {
            m_renderTextureMap[name].Load(width, height);
        }
    }

    void SpriteManager::LoadSprite(const std::string& name, const std::string& fileName, const int width, const int height, bool aspectRatio)
    {
        m_spriteMap[name].Load(fileName, width, height, aspectRatio);
    }

    void SpriteManager::LoadSprite(const std::string& name, Image& image, const int width, const int height, bool aspectRatio)
    {
        m_spriteMap[name].Load(image, width, height, aspectRatio);
    }

    void SpriteManager::UpdateSprite(std::string name, std::string fileName, const int width, const int height, bool aspectRatio)
    {
        m_spriteMap[name].Unload();
        m_spriteMap[name].Load(fileName, width, height, aspectRatio);
    }

    Texture* SpriteManager::GetTexture(const std::string& name)
    {
        std::string find = name;
        if (name == "sprite")
        {
            find = ThemesManager::Get().GetSpriteTheme();
        }

        auto it = m_spriteMap.find(find);
        if (it == m_spriteMap.end())
        {
            return m_spriteMap["transparent"].GetTexture();
        }
        return m_spriteMap[find].GetTexture();
    }

    RenderTexture* SpriteManager::GetRenderTexture(const std::string& name)
    {
        auto it = m_renderTextureMap.find(name);
        if (it == m_renderTextureMap.end())
        {
            return nullptr;
        }
        return &m_renderTextureMap[name];
    }

    Image* SpriteManager::GetImage(std::string name)
    {
        return m_spriteMap[name].GetImage();
    }

    bool SpriteManager::DeleteSprite(std::string name)
    {
        auto it = m_spriteMap.find(name);
        if (it != m_spriteMap.end())
        {
            it->second.Unload();
            m_spriteMap.erase(it);
            return true;
        }
        return false;
    }

    int SpriteManager::NumSpritesLoaded()
    {
        return static_cast<int>(m_spriteMap.size());
    }

    void SpriteManager::Unload()
    {
        m_spriteMap.clear();
        m_renderTextureMap.clear();
    }

} // namespace ClassicLauncher
