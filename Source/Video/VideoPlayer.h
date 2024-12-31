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
        Image image[2];        // Imagem que será atualizada
        std::mutex frameMutex[2];
        bool frameLock[2];
	    int	frameId;
        int countFrame;

        VideoContext()
        {
            image[0] = Image();
            image[1] = Image();
            frameLock[0] = false;
            frameLock[1] = false;
            frameId = 0;
            countFrame = 0;
        }
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
        Texture2D texture;  // Textura da imagem para renderizar na tela
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
