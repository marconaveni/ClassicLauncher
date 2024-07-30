#include "UtilsFunctionLibrary.h"




void UtilsFunctionLibrary::SetSizeWithProportion(Vector2& texture, const int widthResize, const int heightResize)
{

	// Define a nova largura e altura desejadas
	float newWidth = static_cast<float>(widthResize);  // Largura desejada
	float newHeight = static_cast<float>(heightResize); // Altura desejada

	// Calcula a proporção da imagem original
	const float aspectRatio = texture.x / texture.y;

	// Ajusta as dimensões para manter a proporção
	if (newWidth / aspectRatio > newHeight) {
		newWidth = newHeight * aspectRatio;
	}
	else {
		newHeight = newWidth / aspectRatio;
	}


	texture = Vector2{ newWidth, newHeight };

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

