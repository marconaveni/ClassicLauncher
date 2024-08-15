#include "ImageLoader.h"
#include <thread>
#include <utility>

#include "GameListManager.h"
#include "Math.h"
#include "Print.h"
#include "TextureManager.h"
#include "UtilsFunctionLibrary.h"


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
		std::thread loadThread(&ImageLoader::LoadImage, this, map);
		loadThread.detach(); // Detach the thread so it runs independently
	}
}

void ImageLoader::LoadImage(std::map<int, std::string> map)
{

	for (auto itr = map.begin(); itr != map.end(); ++itr) {


		if(IsTextureReady(*TextureManager::GetInstance()->GetCover(itr->first)))
		{
			continue;
		}
		Image image = ::LoadImage(itr->second.c_str());
		Image imageMini = ImageCopy(image);
		int key = itr->first;

		ImageResize(image, 228, 204);
		ImageResize(imageMini, 28, 40);

		// Add the callback to the queue
		{
			std::lock_guard<std::mutex> lock(queueMutex);
			callbackQueue.emplace([this, image, imageMini, key]() { callbackLoadTexture(image, imageMini, key); });
		}
		cv.notify_one();

	}

}

void ImageLoader::CreateTextures(Image& image, Image& imageMini, int indexGame)
{
	TextureManager::GetInstance()->SetCover(indexGame, image);
	TextureManager::GetInstance()->SetCoverMini(indexGame, imageMini);
	UnloadImage(image);
	UnloadImage(imageMini);
}

void ImageLoader::SetCallbackLoadTexture(std::function<void(Image, Image, int)> callback)
{
	callbackLoadTexture = std::move(callback);
}

void ImageLoader::SetCallbackUnloadTexture(std::function<void(std::vector<int>&, int)> callback)
{
	callbackUnloadTexture = callback;
}

void ImageLoader::ImageResize(Image& image, const int newWidth, const int newHeight)
{
	Vector2 newSize{ static_cast<float>(image.width), static_cast<float>(image.height) };
	UtilsFunctionLibrary::SetSizeWithProportion(newSize, newWidth, newHeight);
	::ImageResize(&image, static_cast<int>(newSize.x), static_cast<int>(newSize.y));
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
