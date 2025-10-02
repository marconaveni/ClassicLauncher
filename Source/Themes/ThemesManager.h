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
    class ConfigurationManager;

    class ThemesManager
    {
    public:

        ThemesManager(GameListManager* gameListManager,
                      SpriteManager* spriteManager,
                      EntityManager* entityManagerRef,
                      ConfigurationManager* configManager);
        ~ThemesManager();
        void Init();
        void LoadTheme();
        static ThemesManager& Get();
        static float GetScaleRenderer();
        ConfigurationThemes mConfigurationThemes;

    private:

        GameListManager* m_gameListManager;
        SpriteManager* m_spriteManager;
        EntityManager* m_entityManagerRef;
        ConfigurationManager* m_configManagerRef;

        std::vector<std::string> GetThemeDirs();
        std::string m_currentSystemName;
        std::string m_lastPathLoaded;
        float m_scaleTexture;
        float m_scaleSystem;
        std::string m_pathThemeSystem;
        static bool GetPathTheme(std::string& file,
                                 int monitorWidth,
                                 int monitorCompare,
                                 const std::string& path,
                                 float numScale);
        float GetSpriteByResolution(std::string& file);
        void LoadConfigurationThemes();
    };

} // namespace ClassicLauncher

#endif // THEME_H