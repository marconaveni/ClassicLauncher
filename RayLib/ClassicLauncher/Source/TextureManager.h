#pragma once
#include <map>
#include <string>
#include "RaylibCpp.h"


class TextureManager
{

	TextureManager() = default;

	std::map<std::string, TextureImage> sprites;
	std::map<int, TextureImage> covers;
	std::map<int, TextureImage> coversMini;

public:

	static TextureManager* GetInstance();

	void LoadResources();

	Texture2D* GetSprite(const std::string& name);
	Texture2D* GetCover(const int id);
	Texture2D* GetCoverMini(const int id);

	StatusImage GetStatusImage(const std::string& name);

	void SetSprite(const std::string& key, const std::string& path);
	void SetSprite(const std::string& key, const Image& image);


	void SetCover(const int key, const std::string& path);
	void SetCover(const int key, const Image& image);


	void SetCoverMini(const int key, const std::string& path);
	void SetCoverMini(const int key, const Image& image);

	void DeleteSprite(const std::vector<int>& range);

	void ClearCovers();

	void EndPlay();
	
};
