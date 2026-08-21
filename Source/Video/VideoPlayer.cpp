#include "VideoPlayer.h"

#include <vlc/vlc.h>

#include "Utils/Log.h"
#include "Utils/Math.h"
#include "Utils/Utils.h"
#include "Window/Window.h"


namespace ClassicLauncher
{
    libvlc_instance_t* VideoPlayer::m_VLC = nullptr;

    // VLC prepares to render a video frame.
    void* LockPlayer(void* data, void** p_pixels)
    {
        struct VideoContext* c = (struct VideoContext*)data;

        int frame = (c->frameId ^ 1);

        c->frameMutex[frame].lock();
        c->frameLock[frame] = false;
        *p_pixels = c->image[frame].data; // Allocates the pointer to the pixels of the image.
        return NULL;                      // Picture identifier, not needed here.
    }

    // VLC has just rendered a video frame.
    void UnlockPlayer(void* data, void* id, void* const* p_pixels)
    {
        struct VideoContext* c = (struct VideoContext*)data;

        int frame = (c->frameId ^ 1);

        c->frameId = frame;
        c->frameLock[frame] = true;
        c->countFrame++;
        LOG(LogTrace, "frame %d is ready \"c->frameLock[%d]\" is unlock", c->countFrame, c->frameLock[frame]);
        c->frameMutex[frame].unlock();
    }

    // VLC wants to display a video frame.
    void display(void* data, void* id)
    {
        if (data == NULL)
        {
            return;
        }
        // struct VideoContext* c = (struct VideoContext*)data;
    }

    void VideoPlayer::StartVLCInstance()
    {
        if (!m_VLC)
        {
            LOG(LogTrace, "Initializing VideoPlayer...");
            char const* vlc_argv[] = {
                "--no-xlib",             // Linux (avoid X11)
                "--quiet",               // suppress logs
                "--no-video-title-show", // remove title
                "--avcodec-fast",        // Reduces memory usage when decoding.
                "--no-stats"             // Avoid collecting statistics.
                // "--verbose=2",        // log level

            };

            int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

            m_VLC = libvlc_new(vlc_argc, vlc_argv); // LibVLC initialization instance
            if (!m_VLC)
            {
                LOG(LogFatal, "LibVLC initialization failure.");
                return;
            }
            LOG(LogTrace, "VideoPlayer initialized.");
        }
    }

    VideoPlayer::VideoPlayer()
    {
        StartVLCInstance(); // guarantee that the instance was created
    }

    VideoPlayer::~VideoPlayer()
    {
        LOG(LogTrace, "Destroying VideoPlayer...");
        Unload();
        LOG(LogTrace, "VideoPlayer destroyed.");
    }

    bool VideoPlayer::Init(const std::filesystem::path& path, int width, int height, float scale, bool fill)
    {

        LOG(LogInfo, "Initializing video with path: %s", path.string().c_str());
        if (!std::filesystem::exists(path))
        {
            LOG(LogWarning, "path is not exists.");
            return false;
        }

        LOG(LogTrace, "Trying Parsed video");

        if (m_media || m_mediaPlayer || m_parserThread.joinable())
        {
            Unload();
        }

        if (!m_VLC)
        {
            LOG(LogError, "LibVLC not initializate.");
            return false;
        }

        m_media = libvlc_media_new_path(m_VLC, path.string().c_str());
        if (!m_media)
        {
            LOG(LogError, "m_media initialization failure.");
            return false;
        }

        libvlc_media_add_option(m_media, ":start-time=0.7");

        m_width = Math::Clamp(width, 0, WindowSpecs::Width * scale);
        m_height = Math::Clamp(height, 0, WindowSpecs::Height * scale);
        m_fill = fill;

        m_parsePending = true;
        m_isReadyVlC = false;

        m_parserThread = std::thread(
            [this]()
            {
                libvlc_media_parse_with_options(m_media, libvlc_media_parse_local, 0);
                while (libvlc_media_get_parsed_status(m_media) != libvlc_media_parsed_status_done)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                m_parsePending = false;
            });


        return true;
    }

    bool VideoPlayer::VideoParser()
    {
        LOG(LogTrace, "here");

        if (!m_parsePending && !m_isReadyVlC)
        {

            m_mediaPlayer = libvlc_media_player_new_from_media(m_media);
            if (!m_mediaPlayer)
            {
                LOG(LogError, "m_mediaPlayer initialization failure.\n");
                return false;
            }
            // Get the media metadata so we can find the aspect ratio
            unsigned track_count;
            libvlc_media_track_t** tracks;
            track_count = libvlc_media_tracks_get(m_media, &tracks);

            if (track_count == 0)
            {
                return false;
            }

            for (unsigned track = 0; track < track_count; ++track)
            {
                // libvlc_media_track_t* tr = tracks[track];
                if (tracks[track]->i_type == libvlc_track_video)
                {
                    m_widthVideo = tracks[track]->video->i_width;
                    m_heightVideo = tracks[track]->video->i_height;
                }
                else if (tracks[track]->i_type == libvlc_track_audio) {}
            }
            libvlc_media_tracks_release(tracks, track_count);

            Vector2f textureSize((float)m_widthVideo, (float)m_heightVideo);
            Utils::SetSizeWithProportion(textureSize, m_width, m_height, m_fill);
            m_widthVideo = (int)textureSize.x;
            m_heightVideo = (int)textureSize.y;

            m_context.image[0] = {calloc(m_widthVideo * m_heightVideo * 4, 1), // 4 bytes pixel (RGBA)
                                  m_widthVideo,
                                  m_heightVideo,
                                  1,
                                  PixelFormat::UncompressedR8G8B8A8};

            m_context.image[0].CopyTo(m_context.image[1]);

            m_texture.LoadFromImage(&m_context.image[0]);

            libvlc_video_set_format(m_mediaPlayer, "RGBA", m_widthVideo, m_heightVideo, m_widthVideo * 4);
            libvlc_video_set_callbacks(m_mediaPlayer, LockPlayer, UnlockPlayer, display, &m_context);

            m_isReadyVlC = true;
            m_parsePending = false;
            if (m_callbackReady)
            {
                m_callbackReady(); // Let them know the video is ready.
            }

            LOG(LogTrace, "Parsed video");
        }
        return m_isReadyVlC;
    }

    void VideoPlayer::Play()
    {
        m_isCanPlay = true;
    }

    void VideoPlayer::Pause()
    {
        if (!m_isReadyVlC)
        {
            return;
        }

        libvlc_media_player_pause(m_mediaPlayer);
    }

    void VideoPlayer::Resume()
    {
        if (!m_isReadyVlC)
        {
            return;
        }

        libvlc_media_player_play(m_mediaPlayer);
    }

    void VideoPlayer::Stop()
    {
        if (!m_isReadyVlC)
        {
            return;
        }

        libvlc_media_player_stop(m_mediaPlayer);
    }

    void VideoPlayer::Update()
    {
        VideoParser();


        if (!m_isReadyVlC)
        {
            return;
        }

        if (m_isCanPlay)
        {
            m_isCanPlay = false;
            libvlc_media_player_stop(m_mediaPlayer);
            libvlc_media_player_play(m_mediaPlayer);
        }


        int frame = m_context.frameId;

        if (m_context.frameLock[frame])
        {
            m_context.frameMutex[frame].lock();
            m_texture.Update(m_context.image[frame].data);
            m_context.frameLock[frame] = false;
            LOG(LogTrace, "video texture updated %d", m_context.countFrame);
            m_context.frameMutex[frame].unlock();
        }
        else
        {
            LOG(LogTrace, "video texture not updated \"m_context.frameLock[%d]\" is locked", m_context.frameLock[frame]);
        }

        if (IsVideoFinished() && m_isLoop)
        {
            Play();
        }
    }

    void VideoPlayer::Unload()
    {

        if (m_parserThread.joinable())
        {
            m_parserThread.join(); // Wait for the thread to finish.
        }

        m_isReadyVlC = false;
        m_parsePending = false;

        // Release the media player
        if (m_mediaPlayer)
        {
            libvlc_media_player_stop(m_mediaPlayer);
            libvlc_media_player_release(m_mediaPlayer);
            m_mediaPlayer = NULL;
        }

        // Release the media
        if (m_media)
        {
            libvlc_media_release(m_media);
            m_media = NULL;
        }

        // Release raylib resources
        if (m_texture.IsValid())
        {
            m_texture.Unload();
        }
        if (m_context.image[0].IsValid())
        {
            m_context.image[0].Unload();
            m_context.image[0] = Image();
        }
        if (m_context.image[1].IsValid())
        {
            m_context.image[1].Unload();
            m_context.image[1] = Image();
        }
    }

    Texture* VideoPlayer::GetVideoTexture()
    {
        return (m_texture.IsValid()) ? &m_texture : nullptr;
    }

    Sizef VideoPlayer::GetVideoSize()
    {
        return (m_texture.IsValid()) ? m_texture.GetSize() : Sizef{};
    }

    bool VideoPlayer::IsVideoFinished()
    {
        if (!m_isReadyVlC)
        {
            return false;
        }

        const libvlc_state_t state = libvlc_media_player_get_state(m_mediaPlayer);
        return state == libvlc_Ended || state == libvlc_Error;
    }

    bool VideoPlayer::IsVideoPlaying()
    {
        if (!m_isReadyVlC)
        {
            return false;
        }

        const libvlc_state_t state = libvlc_media_player_get_state(m_mediaPlayer);
        return state == libvlc_Playing;
    }

    bool VideoPlayer::IsVideoStopped()
    {
        if (!m_isReadyVlC)
        {
            return false;
        }

        const libvlc_state_t state = libvlc_media_player_get_state(m_mediaPlayer);
        return state == libvlc_Stopped;
    }

    void VideoPlayer::SetVolume(int volume)
    {
        if (!m_isReadyVlC)
        {
            return;
        }

        libvlc_audio_set_volume(m_mediaPlayer, volume);
    }

    void VideoPlayer::PlayerReadyState(std::function<void()> callbackReady)
    {
        m_callbackReady = std::move(callbackReady);
    }

} // namespace ClassicLauncher
