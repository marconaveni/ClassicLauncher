#include "Resources.h"
#include "Core.h"

namespace ClassicLauncher::Resources
{
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

    bool CheckResources()
    {
        return false;
    }


}  // namespace ClassicLauncher::Resources
