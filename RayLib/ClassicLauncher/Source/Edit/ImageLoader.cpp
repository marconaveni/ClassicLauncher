#include "ImageLoader.h"
#include <thread>
#include <utility>

#include "GameListManager.h"
#include "Math.h"
#include "Print.h"
#include "TextureManager.h"
#include "UtilsFunctionLibrary.h"



void ImageLoader::Initialize()
{
	// Set the callback to be called when images are loaded
	threadImage.SetCallback([this](const Image& image, const Image& imageMini, const int indexGame) {
		CreateTextures(image, imageMini, indexGame);
		UnloadImage(image);
		UnloadImage(imageMini);
		});
}

void ImageLoader::Tick()
{
	threadImage.CallbackExecution();
}

ImageLoader* ImageLoader::GetInstance()
{
	static ImageLoader object;
	return &object;
}

void ImageLoader::StartLoadingLoadTexture(const int indexGame)
{

	const int maxLength = GameListManager::GetInstance()->GetGameListSize();
	const int count = Math::Clamp(maxLength, 0, 16);
	std::map<int, std::string> map;
	for (int i = count * -1; i < count; i++)
	{
		const int indexGameFinal = UtilsFunctionLibrary::SetIndexArray(indexGame + i, maxLength);
		const std::string imagePath = GameListManager::GetInstance()->gameList[indexGameFinal].image;
		if (!imagePath.empty())
		{
			map[indexGameFinal] = imagePath;
		}
	}

	if (!map.empty())
	{
		threadImage.StartThread(&ImageLoader::LoadImage, this, map);
	}
}

void ImageLoader::LoadImage(std::map<int, std::string> map)
{

	for (auto itr = map.begin(); itr != map.end(); ++itr) {


		if(TextureManager::GetInstance()->GetCover(itr->first)->statusImage == StatusImage::Loaded)
		{
			continue;
		}
		if (TextureManager::GetInstance()->GetCoverMini(itr->first)->statusImage == StatusImage::Loaded)
		{
			continue;
		}
		Image image = ::LoadImage(itr->second.c_str());
		Image imageMini = ImageCopy(image);
		int key = itr->first;

		UtilsFunctionLibrary::ImageResize(image, 228, 204);
		UtilsFunctionLibrary::ImageResize(imageMini, 28, 40);

		//UnloadImage(image);
		//UnloadImage(imageMini);

		threadImage.Notify(image, imageMini, key);

	}

}

void ImageLoader::CreateTextures(const Image& image, const Image& imageMini, const int indexGame)
{
	TextureManager::GetInstance()->SetCover(indexGame, image);
	TextureManager::GetInstance()->SetCoverMini(indexGame, imageMini);

}

void ImageLoader::UnloadGameListTextureOutRange()
{
	rangeImages.clear();

	const int indexGame = GameListManager::GetInstance()->GetGameId();
	const int maxLength = GameListManager::GetInstance()->GetGameListSize();
	const int count = Math::Clamp(maxLength, 0, 16);
	for (int i = count * -1; i < count; i++)
	{
		const int indexGameFinal = UtilsFunctionLibrary::SetIndexArray(indexGame + i, maxLength);
		rangeImages.push_back(indexGameFinal);
	}

	TextureManager::GetInstance()->DeleteSprite(rangeImages);

}
