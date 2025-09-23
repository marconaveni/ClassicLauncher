#include "SpriteManager.h"

#include "Data/Color.h"

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

    void SpriteManager::LoadSprite(const std::string& name,
                                   const std::string& fileName,
                                   const int width,
                                   const int height,
                                   bool bAspectRatio)
    {
        mSpriteMap[name].Load(fileName, width, height, bAspectRatio);
    }

    void SpriteManager::LoadSprite(const std::string& name,
                                   Image& image,
                                   const int width,
                                   const int height,
                                   bool bAspectRatio)
    {
        mSpriteMap[name].Load(image, width, height, bAspectRatio);
    }

    void SpriteManager::UpdateSprite(std::string name,
                                     std::string fileName,
                                     const int width,
                                     const int height,
                                     bool bAspectRatio)
    {
        mSpriteMap[name].Unload();
        mSpriteMap[name].Load(fileName, width, height, bAspectRatio);
    }

    Texture* SpriteManager::GetTexture(const std::string& name)
    {
        auto it = mSpriteMap.find(name);
        if (it == mSpriteMap.end())
        {     
            return mSpriteMap["transparent"].GetTexture();
        }
        return mSpriteMap[name].GetTexture();
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
        return mSpriteMap[name].GetImage();
    }

    bool SpriteManager::DeleteSprite(std::string name)
    {
        auto it = mSpriteMap.find(name);
        if (it != mSpriteMap.end())
        {
            it->second.Unload();
            mSpriteMap.erase(it);
            return true;
        }
        return false;
    }

    int SpriteManager::NumSpritesLoaded()
    {
        return static_cast<int>(mSpriteMap.size());
    }

    void SpriteManager::UnloadSprites()
    {
        mSpriteMap.clear();
    }

} // namespace ClassicLauncher
