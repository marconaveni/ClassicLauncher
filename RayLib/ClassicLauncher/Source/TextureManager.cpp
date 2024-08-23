#include "TextureManager.h"

#include "Print.h"

TextureManager* TextureManager::GetInstance()
{
	static TextureManager object;
	return &object;
}

void TextureManager::LoadResources()
{
	SetSprite("sprite", "Resources/themes/default/sprite.png");
	SetSprite("ref", "Resources/themes/default/ref.png");
}

Texture2D* TextureManager::GetSprite(const std::string& name)
{
	return &sprites[name].texture;
}

TextureImage* TextureManager::GetCover(const int id)
{
	return &covers[id];
}

TextureImage* TextureManager::GetCoverMini(const int id)
{
	return &coversMini[id];
}

StatusImage TextureManager::GetStatusImage(const std::string& name)
{
	return sprites[name].statusImage;
}

void TextureManager::SetSprite(const std::string& key, const std::string& path)
{
	if (sprites[key].statusImage == StatusImage::Unload)
	{
		sprites[key].LoadTexture(path.c_str());
	}
}

void TextureManager::SetSprite(const std::string& key, const Image& image)
{
	if (sprites[key].statusImage == StatusImage::Unload)
	{
		sprites[key].LoadTexture(image);
	}
}

void TextureManager::SetCover(const int key, const std::string& path)
{
	if (covers[key].statusImage == StatusImage::Unload)
	{
		countClear++;
		covers[key].LoadTexture(path.c_str());
	}
}

void TextureManager::SetCover(const int key, const Image& image)
{
	if (covers[key].statusImage == StatusImage::Unload)
	{
		countClear++;
		covers[key].LoadTexture(image);
	}
}

void TextureManager::SetCoverMini(const int key, const std::string& path)
{
	if (coversMini[key].statusImage == StatusImage::Unload)
	{
		countClear++;
		coversMini[key].LoadTexture(path.c_str());
	}
}

void TextureManager::SetCoverMini(const int key, const Image& image)
{
	if (coversMini[key].statusImage == StatusImage::Unload)
	{
		countClear++;
		coversMini[key].LoadTexture(image);
	}
}

void TextureManager::DeleteSprite(const std::vector<int>& range)
{

	if (countClear > 64)
	{
		countClear = 0;
		//std::vector<int> sid;

		for (auto it = covers.begin(); it != covers.end(); ++it)
		{
			bool bRange = false;
			for (const int& id : range)
			{
				if (it->first == id)
				{
					bRange = true;
					break;
				}
			}
			if (!bRange)
			{
				//PRINT_STRING("Descarregou imagem");
				covers[it->first].statusImage = StatusImage::Unload;
				coversMini[it->first].statusImage = StatusImage::Unload;
				//sid.push_back(it->first);
			}
		}

		//for (const auto& i : sid)
		//{
		//	if (covers[i].statusImage == StatusImage::Unload)
		//	{
		//		//covers.erase(i);
		//		//coversMini.erase(i);
		//		//covers[i].UnloadTexture();
		//		//coversMini[i].UnloadTexture();
		//		PRINT_STRING(TextFormat("Chave removida %d", i));
		//	}
		//}
	}
}

void TextureManager::ClearCovers()
{

	for (auto it = covers.begin(); it != covers.end(); ++it)
	{
		covers[it->first].statusImage = StatusImage::Unload;
	}
	for (auto it = coversMini.begin(); it != coversMini.end(); ++it)
	{
		coversMini[it->first].statusImage = StatusImage::Unload;
	}
}

void TextureManager::EndPlay()
{
	sprites.clear();
	covers.clear();
	coversMini.clear();
}
