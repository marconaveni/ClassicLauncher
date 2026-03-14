#include "ConfigurationManager.h"

#include "Utils/Resources.h"
#include <filesystem>
#include <string_view>

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


    struct Comments
    {
        inline static constexpr std::string_view InternalScale = "# Set internal resolution scale [1 = 1280x720, 2 = 2560x1440] \n# Note: Raspberry Pi 3 uses only 1.";  
        inline static constexpr std::string_view Volume = "# Set audio volume [min = 0, max = 100].";  
        inline static constexpr std::string_view TargetFps = "# Set Framerate limit [0 is unlocked].";  
        inline static constexpr std::string_view Fullscreen = "# Whether to launch the  launcher in fullscreen.";  
        inline static constexpr std::string_view Vsync = "# Enable VSYNC";  
        inline static constexpr std::string_view ClassicLogLevel = "# Classic Logs\n# 8 Display all logs\n# 9 Trace logging\n# 10 Debug logging\n# 11 Info logging\n# 12 Warning logging\n# 13 Error logging\n# 14 Fatal logging\n# 15 Disable logging";  
        inline static constexpr std::string_view RaylibLogLevel = "# Raylib Logs\n# 0 Display all logs\n# 1 Trace logging\n# 2 Debug logging\n# 3  Info logging\n# 4 Warning logging\n# 5 Error logging\n# 6 Fatal logging,\n# 7  Disable logging";  
        inline static constexpr std::string_view ThemeReferenceOverlay = "# Enables a semi-transparent image used only for reference in theme creation. [true or false].";  
        inline static constexpr std::string_view ThemeReferenceImage = "# Reference image path.";  
        inline static constexpr std::string_view SuspendWindow = "# The window should be closed when the launcher starts the emulator [true or false].\n# Note: on Linux in KMS/DRM mode, always leave as true.";  
        inline static constexpr std::string_view WidthWindow = "# Initial window width.";  
        inline static constexpr std::string_view HeightWindow = "# Initial window height.";  
    };

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

    void ConfigurationManager::SetValues(Ini& config)
    {
        config.SetInt("configuration", "internal_scale", m_internalScale, Comments::InternalScale.data());
        config.SetInt("configuration", "volume", m_volume, Comments::Volume.data());
        config.SetInt("configuration", "target_fps", m_targetFps, Comments::TargetFps.data());
        config.SetBoolean("configuration", "fullscreen", m_fullscreen, Comments::Fullscreen.data());
        config.SetBoolean("configuration", "vsync", m_vsync, Comments::Vsync.data());
        config.SetInt(LOG_SECTION_NAME, "classic_log_level", m_classicLogLevel, Comments::ClassicLogLevel.data());
        config.SetInt(LOG_SECTION_NAME, "raylib_log_level", m_raylibLogLevel, Comments::RaylibLogLevel.data());
        config.SetBoolean("themes", "theme_reference_overlay", m_themeReferenceOverlay, Comments::ThemeReferenceOverlay.data());
        config.SetString("themes", "theme_reference_image", m_themeReferenceImage, Comments::ThemeReferenceImage.data());
        config.SetBoolean("window", "suspend_window", m_suspendWindow, Comments::SuspendWindow.data());
        config.SetInt("window", "width_window", m_widthWindow, Comments::WidthWindow.data());
        config.SetInt("window", "height_window", m_heightWindow, Comments::HeightWindow.data());
    }

    void ConfigurationManager::GetValues(Ini& config)
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
        const std::filesystem::path file = Resources::GetConfigurationFile();
        if (!config.Open(file))
        {
            SetValues(config);
            config.Save(file);
            return;
        }

        GetValues(config);
    }

    bool ConfigurationManager::SaveConfiguration()
    {     
        SetValues(config);
        return config.Save(Resources::GetConfigurationFile());
    }

#undef LOG_SECTION_NAME
#undef CLASSIC_LOG_LEVEL
#undef RAYLIB_LOG_LEVEL
#undef FULLSCREEN
#undef SUSPEND_WINDOW

} // namespace ClassicLauncher
