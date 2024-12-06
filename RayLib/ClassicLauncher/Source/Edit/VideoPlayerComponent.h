#pragma once

#include "Object.h"
#include "VideoPlayer.h"

class VideoPlayerComponent 
{
    VideoPlayer videoPlayer;

    Rectangle position;

    bool bCustomSize;

    VideoPlayerComponent()
        : position(Rectangle{ 0,0,640,480 }), bCustomSize(false)
    {}

public:


    static VideoPlayerComponent* GetInstance();

    void PlayVideo(const char* videoPath);

    void BeginPlay();
    void Tick();
    void Draw();
    void EndDraw();
    void EndPlay();
    void Play();
    void Pause();
    void Stop();
    void SetVolume(int volume);
    void SetCustomSize(bool bEnable, Vector2 newSize);
    std::string GetProportion();
    bool IsVideoPlaying();
    void EndVideoCallback();
    
    ~VideoPlayerComponent() = default;
};
