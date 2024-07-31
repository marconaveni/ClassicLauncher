#include "ImageLoader.h"
#include <thread>

#include "GameListManager.h"
#include "Math.h"
#include "Print.h"


ImageLoader* ImageLoader::GetInstance()
{
	static ImageLoader object;
	return &object;
}

void ImageLoader::LoadImage(const char* path, Vector2 size, int index)
{
	for (int i = 0; i < 30; i++)
	{
		const int indexFinal = Math::Clamp(index + i, 0, static_cast<int>(GameListManager::GetInstance()->gameList.size()) - 1);
		if (!IsTextureReady(GameListManager::GetInstance()->gameList[indexFinal].texture))
		{
			// Add the callback to the queue
			{
				std::string pathImage = GameListManager::GetInstance()->gameList[indexFinal].image;
				Image img = ::LoadImage(pathImage.c_str());
				//ImageResize(&img, 50, 50);
				std::lock_guard<std::mutex> lock(queueMutex);
				callbackQueue.push([this, img, indexFinal]() { callback(img, indexFinal, false); });
			}
			cv.notify_one();

		}
	}
}

void ImageLoader::StartLoading(const char* path, Vector2 size, int index)
{
	std::thread loadThread(&ImageLoader::LoadImage, this, path, size, index);
	loadThread.detach(); // Detach the thread so it runs independently
}

void ImageLoader::SetCallback(std::function<void(Image, int, bool)> callback)
{
	this->callback = callback;
}

void ImageLoader::CreateTextures(const Image& image, int index, bool bCond)
{
	PRINT_STRING("Executou callback");

	Texture2D texture = LoadTextureFromImage(image);
	UnloadTexture(GameListManager::GetInstance()->gameList[index].texture);
	GameListManager::GetInstance()->gameList[index].texture = texture;

	UnloadImage(image);
}
