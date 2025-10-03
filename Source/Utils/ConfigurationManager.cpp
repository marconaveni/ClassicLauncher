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
        : m_internalScale(2)
        , m_forceInternalScale(false)
        , m_volume(100)
        , m_targetFps(60)
        , m_classicLogLevel(CLASSIC_LOG_LEVEL)
        , m_raylibLogLevel(RAYLIB_LOG_LEVEL)
        , m_vsync(true)
        , m_fullscreen(FULLSCREEN)
    {
    }

    ConfigurationManager::~ConfigurationManager()
    {
        SaveConfiguration();
    }

    void ConfigurationManager::SetValues(SimpleIni& config)
    {
        config.SetInt("configuration", "InternalScale", m_internalScale);
        config.SetBoolean("configuration", "ForceInternalScale", m_forceInternalScale);
        config.SetInt("configuration", "Volume", m_volume);
        config.SetInt("configuration", "TargetFps", m_targetFps);
        config.SetBoolean("configuration", "VSync", m_vsync);
        config.SetInt(LOG_SECTION_NAME, "ClassicLogLevel", m_classicLogLevel);
        config.SetInt(LOG_SECTION_NAME, "RaylibLogLevel", m_raylibLogLevel);
        config.SetBoolean("configuration", "Fullscreen", m_fullscreen);
    }

    void ConfigurationManager::GetValues(SimpleIni& config)
    {
        m_internalScale = config.GetInt("configuration", "InternalScale", 2);
        m_forceInternalScale = config.GetBoolean("configuration", "ForceInternalScale", false);
        m_volume = config.GetInt("configuration", "Volume", 100);
        m_targetFps = config.GetInt("configuration", "TargetFps", 60);
        m_vsync = config.GetBoolean("configuration", "VSync", true);
        m_classicLogLevel = config.GetInt(LOG_SECTION_NAME, "ClassicLogLevel", CLASSIC_LOG_LEVEL);
        m_raylibLogLevel = config.GetInt(LOG_SECTION_NAME, "RaylibLogLevel", RAYLIB_LOG_LEVEL);
        m_fullscreen = config.GetBoolean("configuration", "Fullscreen", FULLSCREEN);
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
