#include "SpriteManager.h"


namespace ClassicLauncher
{
    SpriteManager::SpriteManager()
    {
    }

    void SpriteManager::Init()
    {
        Image transparentImage = GenImageColor(1, 1, Color::Blank());
        Image blackImage = GenImageColor(1, 1, Color::Black());
        LoadSprite("transparent", transparentImage);
        LoadSprite("black", blackImage);
        UnloadImage(transparentImage);
        UnloadImage(blackImage);
    }

    void SpriteManager::LoadSprite(const std::string& name, const std::string& fileName, const int width, const int height, bool bAspectRatio)
    {
        mSpriteMap[name].Load(fileName, width, height, bAspectRatio);
    }

    void SpriteManager::LoadSprite(const std::string& name, const Image& image, const int width, const int height, bool bAspectRatio)
    {
        mSpriteMap[name].Load(image, width, height, bAspectRatio);
    }

    void SpriteManager::UpdateSprite(std::string name, std::string fileName, const int width, const int height, bool bAspectRatio)
    {
        mSpriteMap[name].Unload();
        mSpriteMap[name].Load(fileName, width, height, bAspectRatio);
    }

    Texture2D* SpriteManager::GetTexture(const std::string& name)
    {
        auto it = mSpriteMap.find(name);
        if (it == mSpriteMap.end())
        {
            return mSpriteMap["transparent"].GetTexture();
        }
        return mSpriteMap[name].GetTexture();
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
        return static_cast<int>(mSpriteMap.size()) ;
    }

    void SpriteManager::UnloadSprites()
    {
        mSpriteMap.clear();
    }

}  // namespace ClassicLauncher
