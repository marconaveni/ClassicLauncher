#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

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
        [[nodiscard]] bool GetForceInternalScale() const { return m_forceInternalScale; }
        [[nodiscard]] int GetVolume() const { return m_volume; }
        [[nodiscard]] int GetTargetFps() const { return m_targetFps; }
        [[nodiscard]] int GetClassicLogLevel() const { return m_classicLogLevel; }
        [[nodiscard]] int GetRaylibLogLevel() const { return m_raylibLogLevel; }
        [[nodiscard]] bool GetVSync() const { return m_vsync; }
        [[nodiscard]] bool GetFullscreen() const { return m_fullscreen; }

        // Setters
        void SetInternalScale(int internalscale) { m_internalScale = internalscale; }
        void SetForceInternalScale(bool forceInternalScale) { m_forceInternalScale = forceInternalScale; }
        void SetVolume(int volume) { m_volume = volume; }
        void SetTargetFps(int targetFps) { m_targetFps = targetFps; }
        void SetClassicLogLevel(int classicLogLevel) { m_classicLogLevel = classicLogLevel; }
        void SetRaylibLogLevel(int raylibLogLevel) { m_raylibLogLevel = raylibLogLevel; }
        void SetVSync(bool vSync) { m_vsync = vSync; }
        void SetFullscreen(bool bFullscreen) { m_fullscreen = bFullscreen; }

    private:

        void SetValues(SimpleIni& config);
        void GetValues(SimpleIni& config);

        int m_internalScale;
        bool m_forceInternalScale;
        int m_volume;
        int m_targetFps;
        int m_classicLogLevel;
        int m_raylibLogLevel;
        bool m_vsync;
        bool m_fullscreen;
    };

} // namespace ClassicLauncher

#endif