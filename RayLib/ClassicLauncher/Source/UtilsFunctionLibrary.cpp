#include "UtilsFunctionLibrary.h"
#include <algorithm>




void UtilsFunctionLibrary::SetSizeWithProportion(Vector2& texture, const int widthResize, const int heightResize)
{

	// Define a nova largura e altura desejadas
	float newWidth = widthResize;  // Largura desejada
	float newHeight = heightResize; // Altura desejada

	// Calcula a proporção da imagem original
	float aspectRatio = (float)texture.x / (float)texture.y;

	// Ajusta as dimensões para manter a proporção
	if (newWidth / aspectRatio > newHeight) {
		newWidth = newHeight * aspectRatio;
	}
	else {
		newHeight = newWidth / aspectRatio;
	}


	texture = Vector2{ newWidth, newHeight };

}

std::string UtilsFunctionLibrary::NormalizePath(const std::string& path)
{
#ifdef _WIN32
	std::string convertedPath = path;
	std::replace(convertedPath.begin(), convertedPath.end(), '/', '\\');
	return convertedPath;
#else
	return path;
#endif
}

Texture2D UtilsFunctionLibrary::LoadTexture(const std::string& path, int width, int height)
{
	Image img = LoadImage(path.c_str());
	width = (width > 0) ? width : img.width;
	height = (height > 0) ? height : img.height;
	ImageResize(&img, width, height);
	const Texture2D texture = LoadTextureFromImage(img);
	UnloadImage(img);
	return texture;
}

