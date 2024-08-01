#pragma once

#include <vector>
#include "raylib.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>


class ImageLoader
{

private:

	ImageLoader() = default;

	std::vector<Image> covers;

	std::vector<int> rangeImages;


public:

	std::queue<std::function<void()>> callbackQueue;
	std::mutex queueMutex;
	std::condition_variable cv;
	std::function<void(Image, int)> callback;
	std::function<void(std::vector<int>&)> callbackUnloadTexture;

	std::vector<Texture2D> coversTextures;

	Texture2D tileSet;

	static ImageLoader* GetInstance();

	void LoadImage(const char* path, Vector2 size, int index);

	void StartLoadingLoadTexture(const char* path, Vector2 size, int index);

	//void StartLoadingUnLoadTexture(std::vector<int> rangeImages);

	void SetCallbackLoadTexture(std::function<void(Image, int)> callback);

	void SetCallbackUnloadTexture(std::function<void(std::vector<int>&)> callback);

	void CreateTextures(const Image& image, int index);

	void UnloadGameListTextureOutRange(const std::vector<int>& range);
};






