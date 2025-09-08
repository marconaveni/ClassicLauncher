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
        [[nodiscard]]int GetInternalScale() const { return mInternalScale; }
        [[nodiscard]]bool GetForceInternalScale() const { return mForceInternalScale; }
        [[nodiscard]]int GetVolume() const { return mVolume; }
        [[nodiscard]]int GetTargetFps() const { return mTargetFps; }
        [[nodiscard]]int GetClassicLogLevel() const { return mClassicLogLevel; }
        [[nodiscard]]int GetRaylibLogLevel() const { return mRaylibLogLevel; }
        [[nodiscard]]bool GetVSync() const { return mVSync; }
        [[nodiscard]]bool GetFullscreen() const { return mFullscreen; }

        // Setters
        void SetInternalScale(int internalscale) { mInternalScale = internalscale; }
        void SetForceInternalScale(bool forceInternalScale) { mForceInternalScale = forceInternalScale; }
        void SetVolume(int volume) { mVolume = volume; }
        void SetTargetFps(int targetFps) { mTargetFps = targetFps; }
        void SetClassicLogLevel(int classicLogLevel) { mClassicLogLevel = classicLogLevel; }
        void SetRaylibLogLevel(int raylibLogLevel) { mRaylibLogLevel = raylibLogLevel; }
        void SetVSync(bool vSync) { mVSync = vSync; }
        void SetFullscreen(bool bFullscreen) { mFullscreen = bFullscreen; }

    private:

        void SetValues(SimpleIni& config);
        void GetValues(SimpleIni& config);

        int mInternalScale;
        bool mForceInternalScale;
        int mVolume;
        int mTargetFps;
        int mClassicLogLevel;
        int mRaylibLogLevel;
        bool mVSync;
        bool mFullscreen;
    };

}  // namespace ClassicLauncher

#endif