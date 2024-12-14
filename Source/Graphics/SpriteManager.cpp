#include "SpriteManager.h"


namespace ClassicLauncher
{
    SpriteManager::SpriteManager()
    {
    }

    void SpriteManager::LoadSprites(const std::vector<std::map<std::string, std::string>>& paths, const int width, const int height, bool bAspectRatio)
    {
        for(auto& path : paths)
        {
            LoadSprite(path.begin()->first, path.begin()->second, width, height, bAspectRatio);
        }         
    }

    void SpriteManager::LoadSprite(std::string name, std::string fileName, const int width, const int height, bool bAspectRatio)
    {
        spriteMap[name].Load(fileName, width, height, bAspectRatio);
    }

    void SpriteManager::LoadSprite(std::string name, const Image& image, const int width, const int height, bool bAspectRatio)
    {
        spriteMap[name].Load(image, width, height, bAspectRatio);
    }

    void SpriteManager::UpdateSprite(std::string name, std::string fileName, const int width, const int height, bool bAspectRatio)
    {
        spriteMap[name].Unload();
        spriteMap[name].Load(fileName, width, height, bAspectRatio);
    }

    Texture2D* SpriteManager::GetTexture(std::string name)
    {
        return spriteMap[name].GetTexture();
    }

    Image* SpriteManager::GetImage(std::string name)
    {
        return spriteMap[name].GetImage();
    }

    bool SpriteManager::DeleteSprite(std::string name)
    {
        spriteMap.erase(name);
        return false;
    }

    void SpriteManager::UnloadSprites()
    {
        spriteMap.clear();
    }

} // namespace ClassicLauncher
