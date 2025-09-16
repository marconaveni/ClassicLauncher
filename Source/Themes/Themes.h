#ifndef THEME_H
#define THEME_H

#include <string>
#include <vector>

#include "Themes/ConfigurationThemes.h"

namespace ClassicLauncher
{

    class Application;
    class GameListManager;
    class SpriteManager;
    class EntityManager;

    class Themes
    {
    public:

        Themes(GameListManager* gameListManager, SpriteManager* spriteManager, EntityManager* entityManagerRef);
        ~Themes();
        void Init();
        void LoadTheme();
        void LoadConfigurationThemes();
        // static Themes& Get();
        static float GetScaleTexture();
        ConfigurationThemes mConfigurationThemes;

    private:

        GameListManager* m_gameListManager;
        SpriteManager* m_spriteManager;
        EntityManager* m_entityManagerRef;

        std::vector<std::string> GetThemeDirs();
        std::string mCurrentSystemName;
        std::string mLastPathLoaded;
        float mScaleTexture;
        float mScaleSystem;
        std::string mPathThemeSystem;
        static bool GetPathTheme(std::string& file, int monitorWidth, int monitorCompare, const std::string& path,
                                 float numScale);
        float GetSpriteByResolution(std::string& file);
    };

} // namespace ClassicLauncher

#endif // THEME_H