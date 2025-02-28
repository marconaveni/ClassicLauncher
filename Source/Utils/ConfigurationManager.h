#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include <string>

namespace ClassicLauncher
{

    class Configuration;

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

    public:

        ConfigurationManager();
        ~ConfigurationManager();

        bool LoadConfiguration(const std::string path);
        int GetInternalScale() { return mInternalScale; }
        bool GetForceInternalScale() { return mForceInternalScale; }
        int GetVolume() { return mVolume; }
        int GetTargetFps() { return mTargetFps; }
        int GetClassicLogLevel() { return mClassicLogLevel; }
        int GetRaylibLogLevel() { return mRaylibLogLevel; }
        bool GetVSync() { return mVSync; }
    };

}  // namespace ClassicLauncher

#endif