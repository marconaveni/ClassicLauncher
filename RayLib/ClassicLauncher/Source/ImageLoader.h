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

public:

	std::queue<std::function<void()>> callbackQueue;
	std::mutex queueMutex;
	std::condition_variable cv;
	std::function<void(Image,int)> callback;

	std::vector<Texture2D> coversTextures;

	Texture2D tileSet;

	static ImageLoader* GetInstance();

	void LoadImage(const char* path, Vector2 size);

	void StartLoading(const char* path, Vector2 size);

	void SetCallback(std::function<void(Image,int)> callback);

	void CreateTextures();
};






