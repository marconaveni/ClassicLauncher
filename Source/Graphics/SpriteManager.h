#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "Sprite.h"
#include "raylib.h"
#include <map>
#include <string>

namespace ClassicLauncher
{
    class SpriteManager
    {
    private:
        
        std::map<std::string,Sprite> spriteMap;

    public:
        
        SpriteManager();
        void LoadSprite(std::string name, std::string fileName, const int width = 0, const int height = 0, bool bAspectRatio = true);
        void LoadSprite(std::string name, const Image& image, const int width = 0, const int height = 0, bool bAspectRatio = true);
        void UpdateSprite(std::string name, std::string fileName, const int width = 0, const int height = 0, bool bAspectRatio = true);
        Texture2D* GetTexture(std::string name);
        Image* GetImage(std::string name);
        bool DeleteSprite(std::string name);
        void UnloadSprites();
    };
    
}

#endif // SPRITE_MANAGER_H