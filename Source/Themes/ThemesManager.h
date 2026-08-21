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
    class AudioManager;

    class ThemesManager
    {
    public:

        ThemesManager(GameListManager* gameListManager,
                      SpriteManager* spriteManager,
                      EntityManager* entityManagerRef,
                      ConfigurationManager* configManager,
                      AudioManager* m_audioManagerRef);
        ~ThemesManager();
        void Init();
        void UpdateTheme();
        static ThemesManager& Get();
        static float GetScaleRenderer();
        static ConfigurationThemes GetConfigurationThemes();
        std::string GetSpriteTheme();

    private:

        GameListManager* m_gameListManager = nullptr;
        SpriteManager* m_spriteManager = nullptr;
        EntityManager* m_entityManagerRef = nullptr;
        ConfigurationManager* m_configManagerRef = nullptr;
        AudioManager* m_audioManagerRef = nullptr;

        float m_scaleTexture{1.0f};
    };

} // namespace ClassicLauncher

#endif // THEME_H