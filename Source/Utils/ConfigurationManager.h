#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include <string>

#include "Utils/SimpleIni.h"

namespace ClassicLauncher
{

    class ConfigurationManager
    {
    public:

        ConfigurationManager();
        ~ConfigurationManager();

        void LoadConfiguration();
        bool SaveConfiguration();

        // Getters
        [[nodiscard]] int GetInternalScale() const { return m_internalScale; }
        [[nodiscard]] int GetVolume() const { return m_volume; }
        [[nodiscard]] int GetTargetFps() const { return m_targetFps; }
        [[nodiscard]] int GetClassicLogLevel() const { return m_classicLogLevel; }
        [[nodiscard]] int GetRaylibLogLevel() const { return m_raylibLogLevel; }
        [[nodiscard]] bool GetVSync() const { return m_vsync; }
        [[nodiscard]] bool GetFullscreen() const { return m_fullscreen; }
        [[nodiscard]] bool GetThemeReferenceOverlay() const { return m_themeReferenceOverlay; }
        [[nodiscard]] std::string GetThemeReferenceImage() const { return m_themeReferenceImage; }

        // Setters
        void SetInternalScale(int internalscale) { m_internalScale = internalscale; }
        void SetVolume(int volume) { m_volume = volume; }
        void SetTargetFps(int targetFps) { m_targetFps = targetFps; }
        void SetClassicLogLevel(int classicLogLevel) { m_classicLogLevel = classicLogLevel; }
        void SetRaylibLogLevel(int raylibLogLevel) { m_raylibLogLevel = raylibLogLevel; }
        void SetVSync(bool vSync) { m_vsync = vSync; }
        void SetFullscreen(bool bFullscreen) { m_fullscreen = bFullscreen; }
        void SetThemeReferenceOverlay(bool bThemeReferenceOverlay) { m_themeReferenceOverlay = bThemeReferenceOverlay; }
        void SetThemeReferenceImage(const std::string& themeReferenceImage) { m_themeReferenceImage = themeReferenceImage; }

    private:

        void SetValues(SimpleIni& config);
        void GetValues(SimpleIni& config);

        int m_internalScale;
        int m_volume;
        int m_targetFps;
        int m_classicLogLevel;
        int m_raylibLogLevel;
        bool m_vsync;
        bool m_fullscreen;
        bool m_themeReferenceOverlay;
        std::string m_themeReferenceImage;
    };

} // namespace ClassicLauncher

#endif