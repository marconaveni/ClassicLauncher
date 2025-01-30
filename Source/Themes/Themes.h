#ifndef THEME_H
#define THEME_H

#include <string>
#include <vector>
#include "Core.h"

namespace ClassicLauncher
{

    class Application;
    //struct SystemList;

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
        void Init(Application* pApplication);
        void LoadTheme(Application* pApplication);
        static Themes& Get();
        static float GetScaleTexture();
    };

}  // namespace ClassicLauncher

#endif  // THEME_H