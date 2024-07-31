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

	int indexCurrent;
	int indexRight;
	int indexLeft;


public:

	std::queue<std::function<void()>> callbackQueue;
	std::mutex queueMutex;
	std::condition_variable cv;
	std::function<void(Image, int, bool)> callback;

	std::vector<Texture2D> coversTextures;

	Texture2D tileSet;

	static ImageLoader* GetInstance();

	void LoadImage(const char* path, Vector2 size, int index);

	void StartLoading(const char* path, Vector2 size, int index);

	void SetCallback(std::function<void(Image, int, bool)> callback);

	void CreateTextures(const Image& image, int index, bool bCond);
};






