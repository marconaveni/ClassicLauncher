#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "Data/Vector2.h"
#include "Graphics/Image.h"


namespace ClassicLauncher::Utils
{

    void SetSizeWithProportionFit(Vector2f& vector, const int widthResize, const int heightResize);
    void SetSizeWithProportionFill(Vector2f& vector, const int widthResize, const int heightResize);
    void SetSizeWithProportion(Vector2f& vector, const int widthResize, const int heightResize, bool bFill = false);
    void ImageResize(Image& image, const int newWidth, const int newHeight);
    void ImageResizeNN(Image& image, const int newWidth, const int newHeight);
    int SetIndexArray(const int index, const int maxArrayLength);
    //std::string GetWorkingDirectory();
    //std::string GetHomeDir();
    //bool ChangeDirectory(const std::string& path);
    int CountChars(const std::string& text, const char compareChar);

} // namespace ClassicLauncher::Utils

#endif
