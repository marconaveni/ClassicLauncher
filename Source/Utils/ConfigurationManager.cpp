#include "ConfigurationManager.h"

#include "Utils/Resources.h"

namespace ClassicLauncher
{

#ifdef _DEBUG
#define LOG_SECTION_NAME "debug"
#define CLASSIC_LOG_LEVEL 10
#define RAYLIB_LOG_LEVEL 4
#define FULLSCREEN false
#else
#define LOG_SECTION_NAME "log"
#define CLASSIC_LOG_LEVEL 13
#define RAYLIB_LOG_LEVEL 5
#define FULLSCREEN true
#endif // _DEBUG

#ifdef _WIN32
#define SUSPEND_WINDOW false 
#else
#define SUSPEND_WINDOW true 
#endif // _WIN32


    ConfigurationManager::ConfigurationManager()
        : m_classicLogLevel(CLASSIC_LOG_LEVEL)
        , m_raylibLogLevel(RAYLIB_LOG_LEVEL)
        , m_fullscreen(FULLSCREEN)
    {
        LoadConfiguration();
    }

    ConfigurationManager::~ConfigurationManager()
    {
        SaveConfiguration();
    }

    void ConfigurationManager::SetValues(SimpleIni& config)
    {
        config.SetInt("configuration", "internal_scale", m_internalScale);
        config.SetInt("configuration", "volume", m_volume);
        config.SetInt("configuration", "target_fps", m_targetFps);
        config.SetBoolean("configuration", "fullscreen", m_fullscreen, "# whats");
        config.SetBoolean("configuration", "vsync", m_vsync);
        config.SetInt(LOG_SECTION_NAME, "classic_log_level", m_classicLogLevel);
        config.SetInt(LOG_SECTION_NAME, "raylib_log_level", m_raylibLogLevel);
        config.SetBoolean("themes", "theme_reference_overlay", m_themeReferenceOverlay);
        config.SetString("themes", "theme_reference_image", m_themeReferenceImage);
        config.SetBoolean("window", "suspend_window", m_suspendWindow);
        config.SetInt("window", "width_window", m_widthWindow);
        config.SetInt("window", "height_window", m_heightWindow);
    }

    void ConfigurationManager::GetValues(SimpleIni& config)
    {
        m_internalScale = config.GetInt("configuration", "internal_scale", 2);
        m_volume = config.GetInt("configuration", "volume", 100);
        m_targetFps = config.GetInt("configuration", "target_fps", 60);
        m_fullscreen = config.GetBoolean("configuration", "fullscreen", FULLSCREEN);
        m_vsync = config.GetBoolean("configuration", "vsync", true);
        m_classicLogLevel = config.GetInt(LOG_SECTION_NAME, "classic_log_level", CLASSIC_LOG_LEVEL);
        m_raylibLogLevel = config.GetInt(LOG_SECTION_NAME, "raylib_log_level", RAYLIB_LOG_LEVEL);
        m_themeReferenceOverlay = config.GetBoolean("themes", "theme_reference_overlay", false);
        m_themeReferenceImage = config.GetString("themes", "theme_reference_image", "none");
        m_suspendWindow = config.GetBoolean("window", "suspend_window", SUSPEND_WINDOW);
        m_widthWindow = config.GetInt("window", "width_window", 1280);
        m_heightWindow = config.GetInt("window", "height_window", 720);
    }

    void ConfigurationManager::LoadConfiguration()
    {
        const std::string path = Resources::GetConfigurationFile();

        if (!config.Open(path.c_str()))
        {
            SetValues(config);
            config.Save(path.c_str());
            return;
        }

        GetValues(config);
    }

    bool ConfigurationManager::SaveConfiguration()
    {      
        SetValues(config);
        return config.Save(Resources::GetConfigurationFile().c_str());
    }

#undef LOG_SECTION_NAME
#undef CLASSIC_LOG_LEVEL
#undef RAYLIB_LOG_LEVEL
#undef FULLSCREEN
#undef SUSPEND_WINDOW

} // namespace ClassicLauncher
