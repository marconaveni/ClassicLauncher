
#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>

namespace ClassicLauncher::Resources
{
    std::string GetResourcesPathFileAbs(const std::string& relativePath);
    std::string GetClickAudio();
    std::string GetCursorAudio();
    std::string GetFont();
    std::string GetSprite();
    std::string GetIcon(int size);
    std::string GetLogo();
    std::string GetClassicLauncherDir();
    void SetClassicLauncherDir();

    bool CheckResources();

}  // namespace ClassicLauncher::Resources

#endif
