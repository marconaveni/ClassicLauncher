#include "ConfigurationManager.h"
#include "Core.h"
#include "Utils/SimpleIni.h"

namespace ClassicLauncher
{

#ifdef _DEBUG
    #define LOG_SECTION_NAME "debug"
    #define CLASSIC_LOG_LEVEL 10
    #define RAYLIB_LOG_LEVEL 4
#else
    #define LOG_SECTION_NAME "log"
    #define CLASSIC_LOG_LEVEL 13
    #define RAYLIB_LOG_LEVEL 5
#endif

    ConfigurationManager::ConfigurationManager()
        : mInternalScale(2)
        , mForceInternalScale(false)
        , mVolume(100)
        , mTargetFps(60)
        , mClassicLogLevel(CLASSIC_LOG_LEVEL)
        , mRaylibLogLevel(RAYLIB_LOG_LEVEL)
        , mVSync(true)
    {
    }

    ConfigurationManager::~ConfigurationManager()
    {
    }

    bool ConfigurationManager::LoadConfiguration(const std::string path)
    {
        SimpleIni config;

        if (!config.Open(path.c_str()))
        {
            config.SetInt("configuration", "InternalScale", mInternalScale);
            config.SetBoolean("configuration", "ForceInternalScale", mForceInternalScale);
            config.SetInt("configuration", "Volume", mVolume);
            config.SetInt("configuration", "TargetFps", mTargetFps);
            config.SetBoolean("configuration", "VSync", mVSync);
            config.SetInt(LOG_SECTION_NAME, "ClassicLogLevel", mClassicLogLevel);
            config.SetInt(LOG_SECTION_NAME, "RaylibLogLevel", mRaylibLogLevel);

            config.Save(path.c_str());

            return false;
        }

        mInternalScale = config.GetInt("configuration", "InternalScale", 2);
        mForceInternalScale = config.GetBoolean("configuration", "ForceInternalScale", false);
        mVolume = config.GetInt("configuration", "Volume", 100);
        mTargetFps = config.GetInt("configuration", "TargetFps", 60);
        mVSync = config.GetBoolean("configuration", "VSync", true);
        mClassicLogLevel = config.GetInt(LOG_SECTION_NAME, "ClassicLogLevel", CLASSIC_LOG_LEVEL);
        mRaylibLogLevel = config.GetInt(LOG_SECTION_NAME, "RaylibLogLevel", RAYLIB_LOG_LEVEL);

        return true;
    }

#undef LOG_SECTION_NAME
#undef CLASSIC_LOG_LEVEL
#undef RAYLIB_LOG_LEVEL

}  // namespace ClassicLauncher
