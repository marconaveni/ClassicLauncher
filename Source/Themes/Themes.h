#ifndef THEME_H
#define THEME_H

#include <string>
#include "Core.h"


namespace ClassicLauncher
{

    class Application;

    class Themes
    {

        float mScaleTexture;
        float GetSpriteByResolution(std::string& file); 
    public:

        Themes();
        ~Themes();
        void LoadSprite(Application* pApplication);
        void SetScaleTexture(float scale = 0);
        static Themes& Get();
        static float GetScaleTexture();
    };

}  // namespace ClassicLauncher

#endif  // THEME_H