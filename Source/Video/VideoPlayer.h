#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <string>
#include <mutex>
#include "Core.h"
#include "vlc/vlc.h"

namespace ClassicLauncher
{
    class VideoPlayer;

    struct VideoContext
    {
        Image image;        // Imagem que será atualizada
        Texture2D texture;  // Textura da imagem para renderizar na tela
        bool frameLock = false;
        std::mutex frameMutex;
    };

    class VideoPlayer
    {
    private:

        static libvlc_instance_t* mVLC;
        libvlc_media_t* mMedia = nullptr;
        libvlc_media_player_t* mMediaPlayer = nullptr;
        int mWidthVideo = 1;
        int mHeightVideo = 1;

        VideoContext mContext;
        bool bIsEnabledVlC;
        unsigned int mWidth;
        unsigned int mHeight;
        bool bLoop;

        static void StartVLCInstance();

    public:

        bool mIsPlaying = false;

        VideoPlayer();
        ~VideoPlayer();
        bool Init(std::string path, int width, int height);
        void Play();
        void Pause();
        void Resume();
        void Stop();
        void Update();
        void Unload();
        Texture2D* GetVideoTexture();
        Vector2 GetVideoSize();
        bool IsVideoFinished();
        bool IsVideoPlaying();
        bool IsVideoStopped();
        void SetLoop(bool loop) { bLoop = loop; }
        void SetVolume(int volume);
    };

}  // namespace ClassicLauncher

#endif
