#ifndef CONFIGURATION_THEMES_H
#define CONFIGURATION_THEMES_H

 
#include <string>

namespace ClassicLauncher
{

    struct ConfigurationThemes
    {
        float horizontalCardsSpace = 6.0f; // 0 
        float horizontalCardsPositionX = -1.0f; // 2 
        float horizontalCardsPositionY = 219.0f; // 222.0f

        void LoadConfigurations(std::string path);
    };
    
    
} // namespace ClassicLauncher


#endif