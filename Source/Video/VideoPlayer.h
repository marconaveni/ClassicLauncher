#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <string>
#include "Core.h"
#include "vlc/vlc.h"

namespace ClassicLauncher
{
    struct Context
    {
        Image image;        // Imagem que será atualizada
        Texture2D texture;  // Textura da imagem para renderizar na tela
    };

    struct LibVLC
    {
        libvlc_instance_t* libvlc;
        libvlc_media_t* m;
        libvlc_media_player_t* mp;
        int width = 1;
        int height = 1;
    };

    class VideoPlayer
    {
    private:

        Context mContext;
        LibVLC mVLC;
        bool bIsEnabledVlC;
        unsigned int mWidth;
        unsigned int mHeight;
        bool bLoop;

    public:

        VideoPlayer();
        ~VideoPlayer();
        bool Init(std::string path, int width, int height);
        void Play();
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
