#include "Resources.h"
#include "Utils/StringFunctionLibrary.h"
#include "Utils/UtilsFunctionLibrary.h"
#include "Helper.h"   
#include "rl_wrap.h"
#include <filesystem>


namespace ClassicLauncher::Resources
{

    std::string s_classicLauncherPath;

    std::string GetResourcesPathFileAbs(const std::string& relativePath)
    {
        std::string path;
        path.append(rlw::GetApplicationDirectory());
        path.append(relativePath);
        path = String::NormalizePath(path);
        return path;
    }

    std::string GetDefaultConfigurations()
    {
        return GetResourcesPathFileAbs("Resources/config.cfg");
    }

    std::string GetClickAudio()
    {
        return GetResourcesPathFileAbs("Resources/audio/click.wav");
    }

    std::string GetCursorAudio()
    {
        return GetResourcesPathFileAbs("Resources/audio/cursor.wav");
    }

    std::string GetFont()
    {
        return GetResourcesPathFileAbs("Resources/fonts/roboto.ttf");
    }

    std::string GetSprite()
    {
        return GetResourcesPathFileAbs("Resources/textures/sprite.png");
    }

    std::string GetIcon(int size)
    {
        return GetResourcesPathFileAbs(TEXT("Resources/textures/logo%dx%d.png", size, size));
    }

    std::string GetLogo()
    {
        return GetResourcesPathFileAbs("Resources/textures/logo.png");
    }

    std::string GetClassicLauncherDir()
    {
        if (s_classicLauncherPath.empty())
        {
            SetClassicLauncherDir();
        }
        return s_classicLauncherPath;
    }

    void SetClassicLauncherDir()
    {
#if WIN32
        std::string path = GetResourcesPathFileAbs("portable.txt");  // portable mode is avaliable only windows system
        // if (rlw::FileExists(path.c_str()))
        if (std::filesystem::exists(path))
        {
            s_classicLauncherPath = GetResourcesPathFileAbs(".ClassicLauncher/");
        }
        else
#endif
        {
            s_classicLauncherPath = Utils::GetHomeDir() + ".ClassicLauncher/";
            s_classicLauncherPath = String::NormalizePath(s_classicLauncherPath);
            //if (!rlw::DirectoryExists(sClassicLauncherPath.c_str()))
            if (!std::filesystem::exists(s_classicLauncherPath))
            {
                std::filesystem::create_directory(s_classicLauncherPath); // todo fazer testes 
                //rlw::MakeDirectory(sClassicLauncherPath.c_str());
            }
        }
    }

    bool CheckResources()
    {
        return false;
    }

}  // namespace ClassicLauncher::Resources
