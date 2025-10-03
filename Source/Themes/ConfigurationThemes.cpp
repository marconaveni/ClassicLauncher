#include "ConfigurationThemes.h"

#include "Utils/SimpleIni.h"

namespace ClassicLauncher
{
    void ConfigurationThemes::LoadConfigurations(const std::string& path)
    {
        SimpleIni ini;
        ini.Open(path.c_str());

        horizontalCardsSpace = ini.GetFloat("horizontalcards", "horizontalCardsSpace", 0);              // 0
        horizontalCardsPositionX = ini.GetFloat("horizontalcards", "horizontalCardsPositionX", 2.0f);   // 2
        horizontalCardsPositionY = ini.GetFloat("horizontalcards", "horizontalCardsPositionY", 222.0f); // 222.0f

        offsetImageX = ini.GetFloat("card", "offsetImageX", 12.0f); //12.0f
        offsetImageY = ini.GetFloat("card", "offsetImageY", 12.0f); //12.0f
        offsetDefaultImageX = ini.GetFloat("card", "offsetDefaultImageX", 24.0f); //24.0f
        offsetDefaultImageY = ini.GetFloat("card", "offsetDefaultImageY", 13.0f); //13.0f
        offsetVideoX = ini.GetFloat("card", "offsetVideoX", 12.0f); //12.0f
        offsetVideoY = ini.GetFloat("card", "offsetVideoY", 12.0f); //12.0f

    }

} // namespace ClassicLauncher
