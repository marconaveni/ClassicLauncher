#pragma once
#include <map>
#include <raylib.h>


class TextureManager
{

	TextureManager() = default;

	std::map<const char*, Texture2D> sprites;

public:

	static TextureManager* GetInstance();

	void LoadResources();

	Texture2D* GetSprite(const char* name);

	void EndPlay();
	
};
