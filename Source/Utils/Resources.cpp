#include "Resources.h"
#include "Core.h"

namespace ClassicLauncher::Resources
{

    std::string sClassicLauncherPath;

    std::string GetResourcesPathFileAbs(const std::string& relativePath)
    {
        std::string path;
        path.append(::GetApplicationDirectory());
        path.append(relativePath);
        path = StringFunctionLibrary::NormalizePath(path);
        return path;
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

    std::string GetLogo()
    {
        return GetResourcesPathFileAbs("Resources/textures/logo.png");
    }

    std::string GetClassicLauncherDir()
    {
        if (sClassicLauncherPath.empty())
        {
            SetClassicLauncherDir();
        }
        return sClassicLauncherPath;
    }

    void SetClassicLauncherDir()
    {
        std::string path = GetResourcesPathFileAbs("portable.txt");
        if (FileExists(path.c_str()))
        {
            sClassicLauncherPath = GetResourcesPathFileAbs(".ClassicLauncher/");
        }
        else
        {
            sClassicLauncherPath = UtilsFunctionLibrary::GetHomeDir() + ".ClassicLauncher/";
            sClassicLauncherPath = StringFunctionLibrary::NormalizePath(sClassicLauncherPath);
            if(!DirectoryExists(sClassicLauncherPath.c_str()))
            {
                MakeDirectory(sClassicLauncherPath.c_str());
            }
        }
    }

    bool CheckResources()
    {
        return false;
    }

}  // namespace ClassicLauncher::Resources
