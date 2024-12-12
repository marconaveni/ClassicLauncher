#ifndef UTILS_FUNCTION_LIBRARY_H
#define UTILS_FUNCTION_LIBRARY_H

#include <string>
#include "raylib.h"


namespace ClassicLauncher::UtilsFunctionLibrary
{
	void SetSizeWithProportion(Vector2& texture, const int widthResize, const int heightResize);
	void ImageResize(Image& image, const int newWidth, const int newHeight);
	Texture2D LoadTexture(const std::string& path, int width = 0, int height = 0);
	int SetIndexArray(const int index, const int maxArrayLength);
	void UnloadClearTexture(Texture2D& texture);
	std::string GetWorkingDirectory();
	std::string GetHomeDir();
	bool ChangeDirectory(const std::string& path);
	int CountChars(const std::string& text, const char compareChar);
}

#endif
