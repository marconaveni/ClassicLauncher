#pragma once

#include <vector>
#include "RaylibCpp.h"
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

	Texture2D tileSet;

	static ImageLoader* GetInstance();

	void LoadImage(int index);

	void StartLoadingLoadTexture(int index);

	void SetCallbackLoadTexture(std::function<void(Image, int)> callback);

	void SetCallbackUnloadTexture(std::function<void(std::vector<int>&)> callback);

	void ImageResize(Image& image, const int newWidth, const int newHeight);

	void CreateTextures(Image& image, int index);

	void UnloadGameListTextureOutRange(const std::vector<int>& range);
};






