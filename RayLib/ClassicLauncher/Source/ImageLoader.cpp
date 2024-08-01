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

void ImageLoader::LoadImage(const char* path, Vector2 size, int index)
{
	rangeImages.clear();
	const int maxLength = GameListManager::GetInstance()->GetGameListSize();
	const int count = Math::Clamp(maxLength, 0, 15);
	for (int i = count * -1; i < count; i++)
	{
		const int indexFinal = UtilsFunctionLibrary::SetIndexArray(index + i, maxLength);

		LOG(LOGWARNING, TextFormat("index = %d", index));
		LOG(LOGWARNING, TextFormat("index + i = %d", index + i));
		LOG(LOGWARNING, TextFormat("indexFinal = %d", indexFinal));
		if (!IsTextureReady(GameListManager::GetInstance()->gameList[indexFinal]->texture))
		{
			std::string pathImage = GameListManager::GetInstance()->gameList[indexFinal]->image;
			Image img = ::LoadImage(pathImage.c_str());
			//Vector2 newSize{ Math::Clamp(img.width, 0, 228) , Math::Clamp(img.height, 0, 204) };
			Vector2 newSize{ (float)img.width, (float)img.height };
			UtilsFunctionLibrary::SetSizeWithProportion(newSize, 228, 204);
			ImageResize(&img, newSize.x, newSize.y);

			// Add the callback to the queue
			{
				std::lock_guard<std::mutex> lock(queueMutex);
				callbackQueue.push([this, img, indexFinal]() { callback(img, indexFinal); });
			}
			cv.notify_one();

		}
		rangeImages.push_back(indexFinal);
	}

	// Add the callback to the queue
	{
		callbackQueue.push([this]() { callbackUnloadTexture(rangeImages); });
	}
	cv.notify_one();

}

void ImageLoader::StartLoadingLoadTexture(const char* path, Vector2 size, int index)
{
	std::thread loadThread(&ImageLoader::LoadImage, this, path, size, index);
	loadThread.detach(); // Detach the thread so it runs independently
}

//void ImageLoader::StartLoadingUnLoadTexture(std::vector<int> rangeImages)
//{
//}

void ImageLoader::SetCallbackLoadTexture(std::function<void(Image, int)> callback)
{
	this->callback = callback;
}

void ImageLoader::SetCallbackUnloadTexture(std::function<void(std::vector<int>&)> callback)
{
	this->callbackUnloadTexture = callback;
}

void ImageLoader::CreateTextures(const Image& image, int index)
{
	PRINT_STRING("Executou callback");

	Texture2D texture = LoadTextureFromImage(image);
	UnloadTexture(GameListManager::GetInstance()->gameList[index]->texture);
	GameListManager::GetInstance()->gameList[index]->texture = texture;
	UnloadImage(image);
}

void ImageLoader::UnloadGameListTextureOutRange(const std::vector<int>& range)
{
	for (int i = 0; i < GameListManager::GetInstance()->GetGameListSize(); i++)
	{
		bool bRange = false;
		for (const int& id : range)
		{
			if (i == id)
			{
				bRange = true;
				break;
			}
		}
		if (!bRange && IsTextureReady(GameListManager::GetInstance()->gameList[i]->texture))
		{
			PRINT_STRING("Descarregou imagem");
			UnloadTexture(GameListManager::GetInstance()->gameList[i]->texture);
			GameListManager::GetInstance()->gameList[i]->texture = Texture2D();
		}
	}
}
