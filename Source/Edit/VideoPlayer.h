#pragma once

#include <vlc/vlc.h>
#include "RaylibCpp.h"
#include <functional>




struct Proportion
{

	double proportion;
	unsigned int simplifiedWidth;
	unsigned int simplifiedHeight;


	Proportion(const unsigned int width, const unsigned int height)
		: proportion(0), simplifiedWidth(0), simplifiedHeight(0)
	{
		SetProportion(width, height);
	}

	Proportion() : proportion(0), simplifiedWidth(0), simplifiedHeight(0) {}

	void SetProportion(const unsigned int width, const unsigned int height)
	{
		if (width != simplifiedWidth && height != simplifiedHeight 
			&& width > 0 && height > 0)
		{
			const unsigned int commonDivisor = gcd(width, height);
			proportion = static_cast<double>(width) / height;
			simplifiedWidth = width / commonDivisor;
			simplifiedHeight = height / commonDivisor;
		}
	}

private:



	unsigned int gcd(unsigned int a, unsigned int b) {
		while (b != 0) {
			unsigned int temp = b;
			b = a % b;
			a = temp;
		}
		return a;
	}
};

typedef std::function<void()> EndVideoCallback;

class VideoPlayer
{

private:


	libvlc_instance_t* inst;
	libvlc_media_t* media;
	libvlc_media_player_t* mediaPlayer;

	Image image;
	Texture2D texture;
	Texture2D textureDefault;
	unsigned int width;
	unsigned int height;
	Proportion proportion;

	EndVideoCallback endVideoCallback;

public:

	VideoPlayer();
	
	VideoPlayer(const char* path, bool bAutoPlay = false);

	void SetEndVideoCallback(EndVideoCallback registerEndVideoCallback);
	
	void Init(const char* path, bool bAutoPlay = false);

	void PlayVideo(const char* videoPath);
	
	void DrawVideoFrame(Rectangle videoSize);

	void UnloadVideoFrame();

	void Play();

	void Pause();

	void Stop();

	void CloseVideo();

	void SetVolume(int volume);

	Proportion GetProportion();

	Vector2 GetVideoSize();

	bool IsVideoFinished();
	bool IsVideoPlaying();
	bool IsVideoStopped();
};

