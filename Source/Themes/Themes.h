#ifndef THEME_H
#define THEME_H

#include <string>
#include <vector>

#include "Themes/ConfigurationThemes.h"

namespace ClassicLauncher
{

    class Application;

    class Themes
    {
        std::vector<std::string> GetThemeDirs();
        std::string mCurrentSystemName;
        std::string mLastPathLoaded;
        float mScaleTexture;
        float mScaleSystem;
        std::string mPathThemeSystem;
        static bool GetPathTheme(std::string& file, int monitorWidth, int monitorCompare, const std::string& path,
                                 float numScale);
        float GetSpriteByResolution(std::string& file);


    public:

        Themes();
        ~Themes();
        void Init(Application* pApplication);
        void LoadTheme(Application* pApplication);
        void LoadConfigurationThemes(Application* pApplication);
        // static Themes& Get();
        static float GetScaleTexture();
        ConfigurationThemes mConfigurationThemes;
    };

} // namespace ClassicLauncher

#endif // THEME_H