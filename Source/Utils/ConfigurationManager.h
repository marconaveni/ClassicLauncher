#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "Utils/SimpleIni.h"

namespace ClassicLauncher
{

    class ConfigurationManager
    {
    private:

        int mInternalScale;
        bool mForceInternalScale;
        int mVolume;
        int mTargetFps;
        int mClassicLogLevel;
        int mRaylibLogLevel;
        bool mVSync;
        bool mFullscreen;

        void SetValues(SimpleIni& config);
        void GetValues(SimpleIni& config);

    public:

        ConfigurationManager();
        ~ConfigurationManager();

        void LoadConfiguration();
        bool SaveConfiguration();

        // Getters
        int GetInternalScale() { return mInternalScale; }
        bool GetForceInternalScale() { return mForceInternalScale; }
        int GetVolume() { return mVolume; }
        int GetTargetFps() { return mTargetFps; }
        int GetClassicLogLevel() { return mClassicLogLevel; }
        int GetRaylibLogLevel() { return mRaylibLogLevel; }
        bool GetVSync() { return mVSync; }
        bool GetFullscreen() { return mFullscreen; }

        // Setters
        void SetInternalScale(int internalscale) { mInternalScale = internalscale; }
        void SetForceInternalScale(bool forceInternalScale) { mForceInternalScale = forceInternalScale; }
        void SetVolume(int volume) { mVolume = volume; }
        void SetTargetFps(int targetFps) { mTargetFps = targetFps; }
        void SetClassicLogLevel(int classicLogLevel) { mClassicLogLevel = classicLogLevel; }
        void SetRaylibLogLevel(int raylibLogLevel) { mRaylibLogLevel = raylibLogLevel; }
        void SetVSync(bool vSync) { mVSync = vSync; }
        void SetFullscreen(bool bFullscreen) { mFullscreen = bFullscreen; }
    };

}  // namespace ClassicLauncher

#endif