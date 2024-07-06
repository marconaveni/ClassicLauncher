#pragma once


#include <string>
#include "raylib.h"

class UtilsFunctionLibrary {

public:

	static void SetSizeWithProportion(Vector2& texture, const int widthResize, const int heightResize);



	static std::string NormalizePath(const std::string& path);


	static Texture2D LoadTexture(const std::string& path, int width = 0, int height = 0);


};