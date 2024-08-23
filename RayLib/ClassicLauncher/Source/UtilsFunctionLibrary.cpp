#include "UtilsFunctionLibrary.h"

#include "Math.h"


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

void UtilsFunctionLibrary::ImageResize(Image& image, const int newWidth, const int newHeight)
{
	Vector2 newSize{ static_cast<float>(image.width), static_cast<float>(image.height) };
	SetSizeWithProportion(newSize, newWidth, newHeight);
	::ImageResize(&image, static_cast<int>(newSize.x), static_cast<int>(newSize.y));
}


Texture2D UtilsFunctionLibrary::LoadTexture(const std::string& path, int width, int height)
{
	Image img = LoadImage(path.c_str());
	width = (width > 0) ? width : img.width;
	height = (height > 0) ? height : img.height;
	::ImageResize(&img, width, height);
	const Texture2D texture = LoadTextureFromImage(img);
	UnloadImage(img);
	return texture;
}

int UtilsFunctionLibrary::SetIndexArray(const int index, const int maxArrayLength)
{
	if (index >= maxArrayLength)
	{
		return std::abs(maxArrayLength - index);
	}
	if (index < 0)
	{
		return maxArrayLength - std::abs(index);
	}
	return index;
}

void UtilsFunctionLibrary::UnloadClearTexture(Texture2D& texture)
{
	UnloadTexture(texture);
	texture = Texture2D();
}

