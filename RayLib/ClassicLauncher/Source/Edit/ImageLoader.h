#pragma once

#include <vector>
#include "RaylibCpp.h"
//#include <queue>
//#include <mutex>
//#include <condition_variable>
//#include <functional>
#include <map>
#include "ThreadManager.h"




class ImageLoader
{

private:

	ImageLoader() = default;

	//std::vector<Image> covers;

	std::vector<int> rangeImages;


public:

	ThreadManager <Image, Image, int> threadImage;

	//std::map<int, std::vector<TextureImage>> textures;

	//std::queue<std::function<void()>> callbackQueue;
	//std::mutex queueMutex;
	//std::condition_variable cv;
	//std::function<void(Image, Image, int)> callbackLoadTexture;
	//std::function<void(std::vector<int>&, int)> callbackUnloadTexture;

	/*Texture2D tileSet;*/

	void Initialize();

	void Tick();

	static ImageLoader* GetInstance();

	void StartLoadingLoadTexture(int indexGame);

	void LoadImage(std::map<int, std::string> map);

	void CreateTextures(const Image& image, const Image& imageMini, int indexGame);

	//void SetCallbackLoadTexture(std::function<void(Image, Image, int)> callback);

	//void SetCallbackUnloadTexture(std::function<void(std::vector<int>&, int)> callback);

	void UnloadGameListTextureOutRange();

	//void ImageResize(Image& image, const int newWidth, const int newHeight);
};






