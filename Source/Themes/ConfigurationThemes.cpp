#include "ConfigurationThemes.h"

#include "Utils/Ini.h"
#include "Utils/Utils.h"

namespace ClassicLauncher
{
    void ConfigurationThemes::LoadConfigurations(const std::filesystem::path file)
    {
        Ini ini;
        ini.Open(file);

        // horizontalcards
        horizontalCardsSpace = ini.GetFloat("horizontalcards", "horizontalCardsSpace", 0);              // 0
        horizontalCardsPositionX = ini.GetFloat("horizontalcards", "horizontalCardsPositionX", 2.0f);   // 2
        horizontalCardsPositionY = ini.GetFloat("horizontalcards", "horizontalCardsPositionY", 222.0f); // 222.0f

        // card
        offsetImageX = ini.GetFloat("card", "offsetImageX", 12.0f);               // 12.0f
        offsetImageY = ini.GetFloat("card", "offsetImageY", 12.0f);               // 12.0f
        offsetDefaultImageX = ini.GetFloat("card", "offsetDefaultImageX", 24.0f); // 24.0f
        offsetDefaultImageY = ini.GetFloat("card", "offsetDefaultImageY", 13.0f); // 13.0f
        offsetVideoX = ini.GetFloat("card", "offsetVideoX", 12.0f);               // 12.0f
        offsetVideoY = ini.GetFloat("card", "offsetVideoY", 12.0f);               // 12.0f

        // frame
        offsetLeftFrame = ini.GetFloat("frame", "offsetLeftFrame", 0.0f);     // 0.0f
        offsetRightFrame = ini.GetFloat("frame", "offsetRightFrame", 0.0f);   // 0.0f
        offsetTopFrame = ini.GetFloat("frame", "offsetTopFrame", 0.0f);       // 0.0f
        offsetBottomFrame = ini.GetFloat("frame", "offsetBottomFrame", 0.0f); // 0.0f

        // topBar
        topBarAnimation = ini.GetBoolean("topbar", "topBarAnimation", false); // false

        // minicover
        numCovers = ini.GetInt("minicover", "numCovers", 32);                 // 32
        sizeX = ini.GetFloat("minicover", "sizeX", 28.0f);                    // 28.0f
        sizeY = ini.GetFloat("minicover", "sizeY", 40.0f);                    // 40.0f
        offsetTopArrow = ini.GetFloat("minicover", "offsetTopArrow", 0.0f);   // 0.0f
        offsetTopCover = ini.GetFloat("minicover", "offsetTopCover", 0.0f);   // 0.0f
        offsetLeftCover = ini.GetFloat("minicover", "offsetLeftCover", 0.0f); // 0.0f

        // hintbar
        const std::string color = ini.GetString("hintBarFooter", "hintBarFooterColor", "FFFFFFFF");
        hintBarFooterColor = Utils::HexToColor(color);

        // title
        titleColor = Utils::HexToColor(ini.GetString("title", "titleColor", "FFFFFFFF"));
    }

} // namespace ClassicLauncher
