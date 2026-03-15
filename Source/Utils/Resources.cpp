#include "Resources.h"

#include <filesystem>

#include "Helper.h"
#include "Themes/ThemesManager.h"
#include "Utils/Math.h"
#include "Utils/Platform.h"
#include "Utils/String.h"
#include "Utils/Utils.h"



namespace ClassicLauncher::Resources
{

    std::string s_classicLauncherPath;

    void SetClassicLauncherDirectory()
    {
#if WIN32
        std::string path = GetExecutableDirectory("portable.txt"); // portable mode is avaliable only windows system

        if (std::filesystem::exists(path))
        {
            s_classicLauncherPath = GetExecutableDirectory(".classicLauncher/");
        }
        else
#endif
        {
            s_classicLauncherPath = GetHomeDirectory() + ".classicLauncher/";
            s_classicLauncherPath = String::NormalizePath(s_classicLauncherPath);
            if (!std::filesystem::exists(s_classicLauncherPath))
            {
                std::filesystem::create_directory(s_classicLauncherPath); // Todo Do tests
            }
        }
    }

    std::string GetExecutableDirectory(const std::string& aditionalPath)
    {
        std::string path;
        path.append(Platform::GetExecutableDirectory());
        path.append("/");
        path.append(aditionalPath);
        path = String::NormalizePath(path);
        return path;
    }

    std::string GetClassicLauncherDirectory(const std::string& aditionalPath)
    {
        if (s_classicLauncherPath.empty())
        {
            SetClassicLauncherDirectory();
        }
        std::string path = s_classicLauncherPath;
        path.append("/");
        path.append(aditionalPath);
        path = String::NormalizePath(path);
        return path;
    }

    std::string GetConfigurationFile()
    {
        return GetExecutableDirectory("Resources/config.cfg");
    }

    std::string GetClickAudioFile()
    {
        return GetExecutableDirectory("Resources/audio/click.wav");
    }

    std::string GetCursorAudioFile()
    {
        return GetExecutableDirectory("Resources/audio/cursor.wav");
    }

    std::string GetFontFile()
    {
        return GetExecutableDirectory("Resources/fonts/roboto.ttf");
    }

    std::string GetSpriteFile()
    {
        int size = Math::ToInt(ThemesManager::GetScaleRenderer());
        return GetExecutableDirectory(TEXT("Resources/textures/sprite%dx.png", size));
    }

    std::string GetIconFile(int size)
    {
        return GetExecutableDirectory(TEXT("Resources/textures/logo%dx%d.png", size, size));
    }

    std::string GetLogoFile()
    {
        return GetExecutableDirectory("Resources/textures/logo.png");
    }

    std::string GetMusicDirectory()
    {
        return GetClassicLauncherDirectory("musics");
    }

    std::string GetThemeDirectory()
    {
        return GetClassicLauncherDirectory("themes");
    }

    std::string GetHomeDirectory()
    {
        std::string env = getenv(HOME_DIR);
        env += "/";
        return String::NormalizePath(env);
    }

} // namespace ClassicLauncher::Resources
