#pragma once
#include <map>
#include <string>
#include "RaylibCpp.h"


class TextureManager
{

	TextureManager() = default;

	std::map<std::string, Texture2D> sprites;

public:

	static TextureManager* GetInstance();

	void LoadResources();

	Texture2D* GetSprite(const std::string& name);

	void EndPlay();
	
};
