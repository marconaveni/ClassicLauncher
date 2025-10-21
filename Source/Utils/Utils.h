#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "Data/Vector2.h"
#include "Data/Color.h"
#include "Graphics/Image.h"


namespace ClassicLauncher::Utils
{

    Color HexToColor(const std::string& hexColor);
    void SetSizeWithProportionFit(Vector2f& vector, const int widthResize, const int heightResize);
    void SetSizeWithProportionFill(Vector2f& vector, const int widthResize, const int heightResize);
    void SetSizeWithProportion(Vector2f& vector, const int widthResize, const int heightResize, bool fill = false);
    void ImageResize(Image& image, const int newWidth, const int newHeight);
    void ImageResizeNN(Image& image, const int newWidth, const int newHeight);
    int SetIndexArray(const int index, const int maxArrayLength);
    int CountChars(const std::string& text, const char compareChar);

} // namespace ClassicLauncher::Utils

#endif
