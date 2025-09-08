#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <string>
#include <unordered_map>
#include "Sprite.h"


namespace ClassicLauncher
{
    class SpriteManager
    {

    public:

        SpriteManager();
        void Init();
        void LoadSprite(const std::string& name, const std::string& fileName, int width = 0, int height = 0, bool bAspectRatio = true);
        void LoadSprite(const std::string& name, const rlw::Image& image, int width = 0, int height = 0, bool bAspectRatio = true);
        void UpdateSprite(std::string name, std::string fileName, int width = 0, int height = 0, bool bAspectRatio = true);
        rlw::Texture2D* GetTexture(const std::string& name);
        rlw::Image* GetImage(std::string name);
        bool DeleteSprite(std::string name);
        int NumSpritesLoaded();
        void UnloadSprites();

    private:

        std::unordered_map<std::string, Sprite> mSpriteMap;
    };

}  // namespace ClassicLauncher

#endif  // SPRITE_MANAGER_H