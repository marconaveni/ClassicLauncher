#ifndef CONFIGURATION_THEMES_H
#define CONFIGURATION_THEMES_H


#include <string>

#include "Data/Color.h"

namespace ClassicLauncher
{

    struct ConfigurationThemes
    {

        // gui horizontal card values
        float horizontalCardsSpace{0.0f};
        float horizontalCardsPositionX{2.0f}; // todo vefificar esse deslocamento
        float horizontalCardsPositionY{222.0f};

        // gui card values
        float offsetImageX{12};
        float offsetImageY{12};
        float offsetDefaultImageX{24};
        float offsetDefaultImageY{13};
        float offsetVideoX{12};
        float offsetVideoY{12};

        // gui frame values
        float offsetLeftFrame{0.0f}; 
        float offsetRightFrame{0.0f};
        float offsetTopFrame{0.0f};
        float offsetBottomFrame{720.0f};

        // gui TopBar values
        bool topBarAnimation{false};

        // gui minicover values
        int numCovers{32}; 
        float sizeX{28.0f};
        float sizeY{40.0f};
        float offsetTopArrow{0.0f};
        float offsetTopCover{0.0f};
        float offsetLeftCover{0.0f}; 

        //gui hint bar
        Color hintBarFooterColor{Color::White};

        //gui title
        Color titleColor{Color::White};

        void LoadConfigurations(const std::string& path);
    };


} // namespace ClassicLauncher


#endif