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

Texture2D* TextureManager::GetCover(const int id)
{
	return &covers[id].texture;
}

Texture2D* TextureManager::GetCoverMini(const int id)
{
	return &coversMini[id].texture;
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
		covers[key].LoadTexture(path.c_str());
	}
}

void TextureManager::SetCover(const int key, const Image& image)
{
	if (covers[key].statusImage == StatusImage::Unload)
	{
		covers[key].LoadTexture(image);
	}
}

void TextureManager::SetCoverMini(const int key, const std::string& path)
{
	if (coversMini[key].statusImage == StatusImage::Unload)
	{
		coversMini[key].LoadTexture(path.c_str());
	}
}

void TextureManager::SetCoverMini(const int key, const Image& image)
{
	if (coversMini[key].statusImage == StatusImage::Unload)
	{
		coversMini[key].LoadTexture(image);
	}
}

void TextureManager::DeleteSprite(const std::vector<int>& range)
{

	if (covers.size() > 64)
	{
		std::vector<int> sid;

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
				covers[it->first].UnloadTexture();
				coversMini[it->first].UnloadTexture();
				sid.push_back(it->first);
			}
		}

		for (const auto& i : sid)
		{
			if (covers[i].statusImage == StatusImage::Unload)
			{
				covers.erase(i);
				coversMini.erase(i);
				PRINT_STRING(TextFormat("Chave removida %d", i));
			}
		}
	}
}

void TextureManager::ClearCovers()
{
	covers.clear();
	coversMini.clear();
}

void TextureManager::EndPlay()
{
	sprites.clear();
	covers.clear();
	coversMini.clear();
}
