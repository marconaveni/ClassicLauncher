#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Core.h"
#include "Sprite.h"


namespace ClassicLauncher
{
    class SpriteManager
    {
    private:

        std::unordered_map<std::string, Sprite> mSpriteMap;

    public:

        SpriteManager();
        void Init();
        void LoadSprite(const std::string& name, const std::string& fileName, const int width = 0, const int height = 0, bool bAspectRatio = true);
        void LoadSprite(const std::string& name, const Image& image, const int width = 0, const int height = 0, bool bAspectRatio = true);
        void UpdateSprite(std::string name, std::string fileName, const int width = 0, const int height = 0, bool bAspectRatio = true);
        Texture2D* GetTexture(const std::string& name);
        Image* GetImage(std::string name);
        bool DeleteSprite(std::string name);
        int NumSpritesLoaded();
        void UnloadSprites();
    };

}  // namespace ClassicLauncher

#endif  // SPRITE_MANAGER_H