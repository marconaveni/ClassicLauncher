#include "ConfigurationThemes.h"
#include "Utils/SimpleIni.h"

namespace ClassicLauncher
{
    void ConfigurationThemes::LoadConfigurations(std::string path)
    {
        SimpleIni ini;
        ini.Open(path.c_str());

        horizontalCardsSpace = ini.GetFloat("horizontalcards", "horizontalCardsSpace", 0); //6.0f; // 0 
        horizontalCardsPositionX = ini.GetFloat("horizontalcards", "horizontalCardsPositionX", 2.0f); //-1.0f; // 2 
        horizontalCardsPositionY = ini.GetFloat("horizontalcards", "horizontalCardsPositionY", 222.0f); //219.0f; // 222.0f
        
    }
    
} // namespace ClassicLauncher


