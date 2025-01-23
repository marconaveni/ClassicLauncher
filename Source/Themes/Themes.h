#ifndef THEME_H
#define THEME_H

#include <string>
#include <vector>
#include "Core.h"

namespace ClassicLauncher
{

    class Application;
    class SystemList;

    class Themes
    {
        std::vector<std::string> GetThemeDirs();
        std::string mCurrentSystemName;
        std::string mLastPathLoaded;
        float mScaleTexture;
        float mScaleSystem;
        std::string mPathThemeSystem;
        bool GetPathTheme(std::string& file, int monitorWidth, int monitorCompare, const std::string& path, float numScale);
        float GetSpriteByResolution(std::string& file);


    public:

        Themes();
        ~Themes();
        void Init(Application* pApplication, float customScale = 0.0f);
        void LoadTheme(Application* pApplication, float customScale = 0.0f);
        static Themes& Get();
        static float GetScaleTexture();
    };

}  // namespace ClassicLauncher

#endif  // THEME_H