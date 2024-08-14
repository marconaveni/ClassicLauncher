#pragma once

#include <vector>
#include "RaylibCpp.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <map>


struct TextureImage
{
	int id;
	Texture2D texture;

	TextureImage()
		: id(-1)
		, texture()
	{}

	~TextureImage()
	{
		::UnloadTexture(texture);
	}

	void UnloadTexture()
	{
		::UnloadTexture(texture);
		texture = Texture2D();
		id = -1;
	}

	void LoadTexture(const char* path)
	{
		texture = ::LoadTexture(path);
	}

	TextureImage(const TextureImage&) = default;  	// Copy constructor

	TextureImage(TextureImage&& other) noexcept       // Move constructor
		: id(other.id)
		, texture(other.texture)
	{}

	TextureImage& operator=(const TextureImage&) = default;  // Copy assignment operator

	TextureImage& operator=(TextureImage&& other) noexcept  // Move assignment operator
	{
		if (this != &other)
		{
			id = other.id;  // Move data from 'other' to 'this' object
			texture = other.texture;
		}
		return *this;
	}
};


class ImageLoader
{

private:

	ImageLoader() = default;

	std::vector<Image> covers;

	std::vector<int> rangeImages;


public:

	std::map<int, std::vector<TextureImage>> textures;

	std::queue<std::function<void()>> callbackQueue;
	std::mutex queueMutex;
	std::condition_variable cv;
	std::function<void(Image, int, int)> callback;
	std::function<void(std::vector<int>&, int)> callbackUnloadTexture;

	Texture2D tileSet;

	static ImageLoader* GetInstance();

	void LoadImage(int indexList, int indexGame);

	void StartLoadingLoadTexture(int indexList, int indexGame);

	void SetCallbackLoadTexture(std::function<void(Image, int, int)> callback);

	void SetCallbackUnloadTexture(std::function<void(std::vector<int>&, int)> callback);

	void ImageResize(Image& image, const int newWidth, const int newHeight);

	void CreateTextures(Image& image, int indexList, int indexGame);

	void UnloadGameListTextureOutRange(const std::vector<int>& range, int indexList);
};






