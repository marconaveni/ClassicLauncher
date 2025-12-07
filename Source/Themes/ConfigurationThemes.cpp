#include "ConfigurationThemes.h"

#include "Utils/SimpleIni.h"
#include "Utils/Utils.h"

namespace ClassicLauncher
{
    void ConfigurationThemes::LoadConfigurations(const std::string& path)
    {
        SimpleIni ini;
        ini.Open(path.c_str());

        // horizontalcards
        horizontalCardsSpace = ini.GetFloat("horizontalcards", "horizontalCardsSpace", 0);              // 0
        horizontalCardsPositionX = ini.GetFloat("horizontalcards", "horizontalCardsPositionX", 2.0f);   // 2
        horizontalCardsPositionY = ini.GetFloat("horizontalcards", "horizontalCardsPositionY", 222.0f); // 222.0f

        // card
        offsetImageX = ini.GetFloat("card", "offsetImageX", 12.0f);               //12.0f
        offsetImageY = ini.GetFloat("card", "offsetImageY", 12.0f);               //12.0f
        offsetDefaultImageX = ini.GetFloat("card", "offsetDefaultImageX", 24.0f); //24.0f
        offsetDefaultImageY = ini.GetFloat("card", "offsetDefaultImageY", 13.0f); //13.0f
        offsetVideoX = ini.GetFloat("card", "offsetVideoX", 12.0f);               //12.0f
        offsetVideoY = ini.GetFloat("card", "offsetVideoY", 12.0f);               //12.0f

        // frame
        offsetLeft = ini.GetFloat("frame", "offsetLeft", 0.0f);   //0.0f
        offsetRight = ini.GetFloat("frame", "offsetRight", 0.0f); //0.0f

        // minicover
        numCovers = ini.GetInt("minicover", "numCovers", 32);                 //32
        sizeX = ini.GetFloat("minicover", "sizeX", 28.0f);                    //28.0f
        sizeY = ini.GetFloat("minicover", "sizeY", 40.0f);                    //40.0f
        offsetTopArrow = ini.GetFloat("minicover", "offsetTopArrow", 0.0f);   //0.0f
        offsetTopCover = ini.GetFloat("minicover", "offsetTopCover", 0.0f);   //0.0f
        offsetLeftCover = ini.GetFloat("minicover", "offsetLeftCover", 0.0f); //0.0f

        // hintbar
        hintBarFooterColor = Utils::HexToColor(ini.GetString("hintBarFooter", "hintBarFooterColor", "FFFFFFFF"));
        
        // title
        titleColor = Utils::HexToColor(ini.GetString("title", "titleColor", "FFFFFFFF"));
    }

} // namespace ClassicLauncher
