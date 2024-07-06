#include "ImageLoader.h"
#include <thread>



ImageLoader* ImageLoader::GetInstance()
{
	static ImageLoader object;
	return &object;
}

void ImageLoader::LoadImage(const char* path, Vector2 size)
{

	FilePathList files = LoadDirectoryFiles("D:\\Emulators\\roms\\snes\\usa\\media\\covers");
	for (unsigned int i = 0; i < files.count; i++)
	{
		Image img = ::LoadImage(files.paths[i]);
		//ImageResize(&img, 50, 50);
		covers.push_back(img);
		 
		
		// Add the callback to the queue
		{
			std::lock_guard<std::mutex> lock(queueMutex);
			callbackQueue.push([this, img]() { callback(img); });
		}
		cv.notify_one();

		
	}

	UnloadDirectoryFiles(files);


}

void ImageLoader::StartLoading(const char* path, Vector2 size)
{
	std::thread loadThread(&ImageLoader::LoadImage, this, path, size);
	loadThread.detach(); // Detach the thread so it runs independently
}

void ImageLoader::SetCallback(std::function<void(Image)> callback)
{
	this->callback = callback;
}

void ImageLoader::CreateTextures()
{

	//for (unsigned int i = 0; i < covers.size(); i++)
	//{
	//	Texture2D texture = LoadTextureFromImage(covers[i]);
	//	coversTextures.push_back(texture);
	//	UnloadImage(covers[i]);
	//}
}
