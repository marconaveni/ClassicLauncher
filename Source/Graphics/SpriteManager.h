#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <string>
#include <unordered_map>

#include "Graphics/Image.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderTexture.h"
#include "Sprite.h"


namespace ClassicLauncher
{
    class SpriteManager
    {

    public:

        SpriteManager();
        void Init();
        void LoadRenderTexture(const std::string& name, int width, int height);
        void LoadSprite(const std::string& name, Image& image, int width = 0, int height = 0, bool bAspectRatio = true);
        void LoadSprite(const std::string& name,
                        const std::string& fileName,
                        int width = 0,
                        int height = 0,
                        bool bAspectRatio = true);
        void UpdateSprite(std::string name,
                          std::string fileName,
                          int width = 0,
                          int height = 0,
                          bool bAspectRatio = true);
        Texture* GetTexture(const std::string& name);
        RenderTexture* GetRenderTexture(const std::string& name);
        Image* GetImage(std::string name);
        bool DeleteSprite(std::string name);
        int NumSpritesLoaded();
        void Unload();

    private:

        std::unordered_map<std::string, Sprite> m_spriteMap;
        std::unordered_map<std::string, RenderTexture> m_renderTextureMap;
    };

} // namespace ClassicLauncher

#endif // SPRITE_MANAGER_H