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
#endif


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
        config.SetInt("configuration", "InternalScale", m_internalScale);
        config.SetInt("configuration", "Volume", m_volume);
        config.SetInt("configuration", "TargetFps", m_targetFps);
        config.SetBoolean("configuration", "VSync", m_vsync);
        config.SetInt(LOG_SECTION_NAME, "ClassicLogLevel", m_classicLogLevel);
        config.SetInt(LOG_SECTION_NAME, "RaylibLogLevel", m_raylibLogLevel);
        config.SetBoolean("configuration", "Fullscreen", m_fullscreen);
        config.SetBoolean("themes", "themeReferenceOverlay", m_themeReferenceOverlay);
        config.SetString("themes", "themeReferenceImage", m_themeReferenceImage);
    }

    void ConfigurationManager::GetValues(SimpleIni& config)
    {
        m_internalScale = config.GetInt("configuration", "InternalScale", 2);
        m_volume = config.GetInt("configuration", "Volume", 100);
        m_targetFps = config.GetInt("configuration", "TargetFps", 60);
        m_vsync = config.GetBoolean("configuration", "VSync", true);
        m_classicLogLevel = config.GetInt(LOG_SECTION_NAME, "ClassicLogLevel", CLASSIC_LOG_LEVEL);
        m_raylibLogLevel = config.GetInt(LOG_SECTION_NAME, "RaylibLogLevel", RAYLIB_LOG_LEVEL);
        m_fullscreen = config.GetBoolean("configuration", "Fullscreen", FULLSCREEN);
        m_themeReferenceOverlay = config.GetBoolean("themes", "themeReferenceOverlay", false);
        m_themeReferenceImage = config.GetString("themes", "themeReferenceImage", "none");
    }

    void ConfigurationManager::LoadConfiguration()
    {
        const std::string path = Resources::GetConfigurationFile();
        SimpleIni config;

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
        SimpleIni config;
        SetValues(config);
        return config.Save(Resources::GetConfigurationFile().c_str());
    }

#undef LOG_SECTION_NAME
#undef CLASSIC_LOG_LEVEL
#undef RAYLIB_LOG_LEVEL
#undef FULLSCREEN

} // namespace ClassicLauncher
