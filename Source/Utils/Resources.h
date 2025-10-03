
#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>

namespace ClassicLauncher::Resources
{
    void SetClassicLauncherDirectory();
    std::string GetExecutableDirectory(const std::string& aditionalPath = "");
    std::string GetClassicLauncherDirectory(const std::string& aditionalPath = "");
    std::string GetConfigurationFile();
    std::string GetClickAudioFile();
    std::string GetCursorAudioFile();
    std::string GetFontFile();
    std::string GetSpriteFile();
    std::string GetIconFile(int size);
    std::string GetLogoFile();
    std::string GetMusicDirectory();
    std::string GetThemeDirectory();
    std::string GetHomeDirectory();

} // namespace ClassicLauncher::Resources

#endif
