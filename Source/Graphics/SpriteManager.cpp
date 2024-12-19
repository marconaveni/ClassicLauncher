#include "SpriteManager.h"

namespace ClassicLauncher
{
    SpriteManager::SpriteManager() {}

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
            return nullptr;
        }
        return mSpriteMap[name].GetTexture();
    }

    Image* SpriteManager::GetImage(std::string name) { return mSpriteMap[name].GetImage(); }

    bool SpriteManager::DeleteSprite(std::string name)
    {
        // auto it = spriteMap.find(name);
        // if (it != spriteMap.end())
        // {
        //     it->second.~Sprite();// deleta o objeto Sprite da memória
        //     spriteMap.erase(it); // remove a referência do spriteMap
        //     return true;
        // }

        mSpriteMap[name].Unload();
        mSpriteMap.erase(name);
        return false;
    }

    void SpriteManager::UnloadSprites() { mSpriteMap.clear(); }

}  // namespace ClassicLauncher
