#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <atomic>
#include <filesystem>
#include <functional>
#include <mutex>
#include <string>
#include <thread>

#include "Data/Vector2.h"
#include "Graphics/Image.h"
#include "Graphics/Texture.h"


#ifdef _WIN32
using ssize_t = intptr_t;
#endif

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_player_t;

namespace ClassicLauncher
{
    class VideoPlayer;


    struct VideoContext
    {
        Image image[2]; // Imagem which will be updated.
        std::mutex frameMutex[2];
        bool frameLock[2];
        int frameId{0};
        int countFrame{0};

        VideoContext()
        {
            image[0] = Image();
            image[1] = Image();
            frameLock[0] = false;
            frameLock[1] = false;
        }
    };

    class VideoPlayer
    {
    public:

        static void StartVLCInstance();
        VideoPlayer();
        ~VideoPlayer();
        bool Init(const std::filesystem::path& path, int width, int height, float scale = 1, bool fill = false);
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
        [[nodiscard]] bool IsReady() { return m_isReadyVlC; }
        void PlayerReadyState(std::function<void()> callbackReady);

    private:

        static libvlc_instance_t* m_VLC;
        std::function<void()> m_callbackReady{nullptr};
        libvlc_media_t* m_media{nullptr};
        libvlc_media_player_t* m_mediaPlayer{nullptr};
        int m_widthVideo{1};
        int m_heightVideo{1};

        VideoContext m_context{};
        Texture m_texture{}; // Texture for rendering on screen.
        unsigned int m_width{0};
        unsigned int m_height{0};
        bool m_isLoop{true};
        bool m_fill{false};

        std::thread m_parserThread{};
        std::atomic<bool> m_parsePending{false};

        bool m_isReadyVlC{false};
        bool m_isCanPlay{false};

        bool VideoParser();
    };

} // namespace ClassicLauncher

#endif
