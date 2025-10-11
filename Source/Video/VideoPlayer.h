#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <mutex>
#include <string>

#include "Data/Vector2.h"
#include "Graphics/Image.h"
#include "Graphics/Texture.h"


struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_player_t;

namespace ClassicLauncher
{
    class VideoPlayer;


    struct VideoContext
    {
        Image image[2]; // Imagem que será atualizada
        std::mutex frameMutex[2];
        bool frameLock[2];
        int frameId;
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

        static libvlc_instance_t* m_VLC;
        libvlc_media_t* m_media = nullptr;
        libvlc_media_player_t* m_mediaPlayer = nullptr;
        int m_widthVideo = 1;
        int m_heightVideo = 1;

        VideoContext m_context;
        Texture m_texture; // Textura da imagem para renderizar na tela
        bool m_isEnabledVlC;
        unsigned int m_width;
        unsigned int m_height;
        bool m_isLoop = true;

        static void StartVLCInstance();

    public:

        //bool m_isPlaying = false;

        VideoPlayer();
        ~VideoPlayer();
        bool Init(std::string path, int width, int height, float scale = 1, bool fill = false);
        void Play();
        void Pause();
        void Resume();
        void Stop();
        void Update();
        void Unload();
        Texture* GetVideoTexture();
        Sizef GetVideoSize();
        bool IsVideoFinished();
        bool IsVideoPlaying();
        bool IsVideoStopped();
        void SetLoop(bool loop) { m_isLoop = loop; }
        void SetVolume(int volume);
    };

} // namespace ClassicLauncher

#endif
