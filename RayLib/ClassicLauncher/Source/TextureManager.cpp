#include "TextureManager.h"

#include "UtilsFunctionLibrary.h"

TextureManager* TextureManager::GetInstance()
{
	static TextureManager object;
	return &object;
}

void TextureManager::LoadResources()
{
	sprites["sprite"] = UtilsFunctionLibrary::LoadTexture("Resources/themes/default/sprite.png");
	sprites["ref"] = UtilsFunctionLibrary::LoadTexture("Resources/themes/default/ref.png");
}

Texture2D* TextureManager::GetSprite(const char* name)
{
	return &sprites[name];
}

void TextureManager::EndPlay()
{
	std::map<const char*, Texture2D>::iterator it;
	for (it = sprites.begin(); it != sprites.end(); ++it) {
		UnloadTexture(it->second);
	}
}
