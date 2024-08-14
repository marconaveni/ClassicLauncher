#include "ImageLoader.h"
#include <thread>

#include "GameListManager.h"
#include "Math.h"
#include "Print.h"
#include "UtilsFunctionLibrary.h"


ImageLoader* ImageLoader::GetInstance()
{
	static ImageLoader object;
	return &object;
}

void ImageLoader::LoadImage(int indexList, int indexGame)
{

	const int maxLength = GameListManager::GetInstance()->GetGameListSize();
	const int count = Math::Clamp(maxLength, 0, 16);
	for (int i = count * -1; i < count; i++)
	{
		const int indexGameFinal = UtilsFunctionLibrary::SetIndexArray(indexGame + i, maxLength);

		if (indexList != GameListManager::GetInstance()->idSystemList || indexGame > GameListManager::GetInstance()->GetGameListSize())
		{
			LOG(LOGINFO, "Cancelou LoadImage");
			break;
		}
		GameList game = GameListManager::GetInstance()->gameList[indexGameFinal];

		if (!IsTextureReady(game.texture))
		{

			if (game.image.empty())
			{
				continue;
			}

			// Add the callback to the queue
			{
				Image img = ::LoadImage(game.image.c_str());
				std::lock_guard<std::mutex> lock(queueMutex);
				callbackQueue.push([this, img, indexList, indexGameFinal]() { callback(img, indexList, indexGameFinal); });
			}
			cv.notify_one();

		}
		//rangeImages.push_back(indexGameFinal);
	}

	//// Add the callback to the queue
	//{
	//	callbackQueue.push([this, indexList]() { callbackUnloadTexture(rangeImages, indexList); });
	//}
	//cv.notify_one();

}

void ImageLoader::StartLoadingLoadTexture(int indexList, int indexGame)
{

	if (indexList != GameListManager::GetInstance()->idSystemList || indexGame > GameListManager::GetInstance()->GetGameListSize())
	{
		PRINT_STRING("Cancelou inicio callback");
		return;
	}
	std::thread loadThread(&ImageLoader::LoadImage, this, indexList, indexGame);
	loadThread.detach(); // Detach the thread so it runs independently
}

//void ImageLoader::StartLoadingUnLoadTexture(std::vector<int> rangeImages)
//{
//}

void ImageLoader::SetCallbackLoadTexture(std::function<void(Image, int, int)> callback)
{
	this->callback = callback;
}

void ImageLoader::SetCallbackUnloadTexture(std::function<void(std::vector<int>&, int)> callback)
{
	this->callbackUnloadTexture = callback;
}

void ImageLoader::ImageResize(Image& image, const int newWidth, const int newHeight)
{
	Vector2 newSize{ (float)image.width, (float)image.height };
	UtilsFunctionLibrary::SetSizeWithProportion(newSize, newWidth, newHeight);
	::ImageResize(&image, (int)newSize.x, (int)newSize.y);
}

void ImageLoader::CreateTextures(Image& image, int indexList, int indexGame)
{

	ImageResize(image, 228, 204);
	const Texture2D texture = LoadTextureFromImage(image);
	ImageResize(image, 28, 40);
	const Texture2D textureMini = LoadTextureFromImage(image);

	if (GameListManager::GetInstance()->idSystemList == indexList && indexGame < GameListManager::GetInstance()->GetGameListSize())
	{
		GameList* game = &GameListManager::GetInstance()->gameList[indexGame];
		UnloadTexture(game->textureMini);
		UnloadTexture(game->texture);
		game->textureMini = textureMini;
		game->texture = texture;
	}
	//PRINT_STRING("Executou callback");
	UnloadImage(image);


}

void ImageLoader::UnloadGameListTextureOutRange(const std::vector<int>& range, int indexList)
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


	//return;
	int i = 0;
	while (i < GameListManager::GetInstance()->GetGameListSize())
	{
		bool bRange = false;
		for (const int& id : rangeImages)
		{
			if (i == id)
			{
				bRange = true;
				break;
			}
		}
		if (!bRange && IsTextureReady(GameListManager::GetInstance()->gameList[i].texture))
		{
			PRINT_STRING("Descarregou imagem");
			UtilsFunctionLibrary::UnloadClearTexture(GameListManager::GetInstance()->gameList[i].textureMini);
			UtilsFunctionLibrary::UnloadClearTexture(GameListManager::GetInstance()->gameList[i].texture);
		}
		i++;
	}

}
