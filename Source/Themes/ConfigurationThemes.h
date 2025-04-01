#ifndef CONFIGURATION_THEMES_H
#define CONFIGURATION_THEMES_H

 
#include <string>

namespace ClassicLauncher
{

    struct ConfigurationThemes
    {

        // gui horizontal card values
        float horizontalCardsSpace = 0.0f;
        float horizontalCardsPositionX = 2.0f;
        float horizontalCardsPositionY = 222.0f;
        
        // gui card values
        float offsetImageX = 12;
        float offsetImageY = 12;
        float offsetVideoX = 12;
        float offsetVideoY = 12;


        void LoadConfigurations(std::string path);
    };
    
    
} // namespace ClassicLauncher


#endif