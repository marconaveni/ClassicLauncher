#pragma once


#include <string>
#include "RaylibCpp.h"


class UtilsFunctionLibrary {

public:

	static void SetSizeWithProportion(Vector2& texture, const int widthResize, const int heightResize);

	static void ImageResize(Image& image, const int newWidth, const int newHeight);

	static Texture2D LoadTexture(const std::string& path, int width = 0, int height = 0);

	static int SetIndexArray(const int index, const int maxArrayLength);

	static void UnloadClearTexture(Texture2D& texture);
};