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
        [[nodiscard]] bool GetSuspendWindow() const { return m_suspendWindow; }

        // Setters
        void SetInternalScale(int internalscale) { m_internalScale = internalscale; }
        void SetVolume(int volume) { m_volume = volume; }
        void SetTargetFps(int targetFps) { m_targetFps = targetFps; }
        void SetClassicLogLevel(int classicLogLevel) { m_classicLogLevel = classicLogLevel; }
        void SetRaylibLogLevel(int raylibLogLevel) { m_raylibLogLevel = raylibLogLevel; }
        void SetVSync(bool vSync) { m_vsync = vSync; }
        void SetFullscreen(bool fullscreen) { m_fullscreen = fullscreen; }
        void SetThemeReferenceOverlay(bool themeReferenceOverlay) { m_themeReferenceOverlay = themeReferenceOverlay; }
        void SetThemeReferenceImage(const std::string& themeReferenceImage) { m_themeReferenceImage = themeReferenceImage; }

    private:

        void SetValues(SimpleIni& config);
        void GetValues(SimpleIni& config);

        int m_internalScale{2};
        int m_volume{100};
        int m_targetFps{60};
        int m_classicLogLevel{13};
        int m_raylibLogLevel{5};
        bool m_vsync{true};
        bool m_fullscreen{true};
        bool m_themeReferenceOverlay{false};
        std::string m_themeReferenceImage{};
        bool m_suspendWindow{false};
    };

} // namespace ClassicLauncher

#endif