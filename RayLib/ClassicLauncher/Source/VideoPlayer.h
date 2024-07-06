#pragma once

#include <vlc/vlc.h>
#include <iostream>
#include "raylib.h"
#include <functional>




struct Proportion
{

	double proportion;
	int simplifiedWidth;
	int simplifiedHeight;


	Proportion(int width, int height) 
		: proportion(0), simplifiedWidth(0), simplifiedHeight(0)
	{
		SetProportion(width, height);
	}

	Proportion() : proportion(0), simplifiedWidth(0), simplifiedHeight(0) {}

	void SetProportion(int width, int height) 
	{
		if (width != simplifiedWidth && height != simplifiedHeight 
			&& width > 0 && height > 0)
		{
			const int commonDivisor = gcd(width, height);
			proportion = static_cast<double>(width) / height;
			simplifiedWidth = width / commonDivisor;
			simplifiedHeight = height / commonDivisor;
		}
	}

private:



	int gcd(int a, int b) {
		while (b != 0) {
			int temp = b;
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
	int width;
	int height;
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

