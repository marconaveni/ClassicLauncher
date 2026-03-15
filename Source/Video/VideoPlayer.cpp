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
    void* lock(void* data, void** p_pixels)
    {
        struct VideoContext* c = (struct VideoContext*)data;

        int frame = (c->frameId ^ 1);

        c->frameMutex[frame].lock();
        c->frameLock[frame] = false;
        *p_pixels = c->image[frame].data; // Allocates the pointer to the pixels of the image.
        return NULL;                      // Picture identifier, not needed here.
    }

    // VLC has just rendered a video frame.
    void unlock(void* data, void* id, void* const* p_pixels)
    {
        struct VideoContext* c = (struct VideoContext*)data;

        int frame = (c->frameId ^ 1);

        c->frameId = frame;
        c->frameLock[frame] = true;
        c->countFrame++;
        LOG(LOG_CLASSIC_TRACE, "frame %d is ready \"c->frameLock[%d]\" is unlock", c->countFrame, c->frameLock[frame]);
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
        char const* vlc_argv[] = {
            "--no-xlib",
            "--quiet",               // suppress logs
            "--no-video-title-show", // remove title
            // "--avcodec-fast",     // Reduces memory usage when decoding.
            // "--verbose=2",
            // "--no-stats"          // Avoid collecting statistics.

        };
        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

        if (!m_VLC)
        {
            m_VLC = libvlc_new(vlc_argc, vlc_argv); // LibVLC initialization instance
            if (!m_VLC)
            {
                LOG(LOG_CLASSIC_FATAL, "LibVLC initialization failure.");
            }
        }
    }

    VideoPlayer::VideoPlayer()
    {
        LOG(LOG_CLASSIC_TRACE, "Initializing VideoPlayer...");
        StartVLCInstance();
        LOG(LOG_CLASSIC_TRACE, "VideoPlayer initialized.");
    }

    VideoPlayer::~VideoPlayer()
    {
        LOG(LOG_CLASSIC_TRACE, "Destroying VideoPlayer...");
        Unload();
        LOG(LOG_CLASSIC_TRACE, "VideoPlayer destroyed.");
    }

    bool VideoPlayer::Init(std::string path, int width, int height, float scale, bool fill)
    {
        LOG(LOG_CLASSIC_INFO, "Initializing video with path: %s", path.c_str());
        if (path.empty())
        {
            LOG(LOG_CLASSIC_WARNING, "path is empty.");
            return false;
        }
        if (!m_VLC)
        {
            LOG(LOG_CLASSIC_ERROR, "LibVLC not initializate.");
            return false;
        }

        m_media = libvlc_media_new_path(m_VLC, path.c_str());
        if (!m_media)
        {
            LOG(LOG_CLASSIC_ERROR, "m_media initialization failure.");
            return false;
        }

        m_mediaPlayer = libvlc_media_player_new_from_media(m_media);
        if (!m_mediaPlayer)
        {
            LOG(LOG_CLASSIC_ERROR, "m_mediaPlayer initialization failure.\n");
            return false;
        }

        m_width = Math::Clamp(width, 0, WindowSpecs::Width * scale);
        m_height = Math::Clamp(height, 0, WindowSpecs::Height * scale);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        libvlc_media_parse(m_media); // libvlc_media_parse_with_options() is async function
#pragma GCC diagnostic pop

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
        Utils::SetSizeWithProportion(textureSize, m_width, m_height, fill);
        m_widthVideo = (int)textureSize.x;
        m_heightVideo = (int)textureSize.y;

        m_context.image[0] = {calloc(m_widthVideo * m_heightVideo * 4, 1), // 4 bytes pixel (RGBA)
                              m_widthVideo,
                              m_heightVideo,
                              1,
                              PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

        m_context.image[0].CopyTo(m_context.image[1]);

        m_texture.LoadFromImage(&m_context.image[0]);

        libvlc_video_set_format(m_mediaPlayer, "RGBA", m_widthVideo, m_heightVideo, m_widthVideo * 4);
        libvlc_video_set_callbacks(m_mediaPlayer, lock, unlock, display, &m_context);

        m_isEnabledVlC = m_VLC && m_media && m_mediaPlayer;
        return m_isEnabledVlC;
    }

    void VideoPlayer::Play()
    {
        if (!m_isEnabledVlC)
        {
            return;
        }

        libvlc_media_player_stop(m_mediaPlayer);
        libvlc_media_player_play(m_mediaPlayer);
    }

    void VideoPlayer::Pause()
    {
        if (!m_isEnabledVlC)
        {
            return;
        }

        libvlc_media_player_pause(m_mediaPlayer);
    }

    void VideoPlayer::Resume()
    {
        if (!m_isEnabledVlC)
        {
            return;
        }

        libvlc_media_player_play(m_mediaPlayer);
    }

    void VideoPlayer::Stop()
    {
        if (!m_isEnabledVlC)
        {
            return;
        }

        libvlc_media_player_stop(m_mediaPlayer);
    }

    void VideoPlayer::Update()
    {
        if (!m_isEnabledVlC)
        {
            return;
        }

        int frame = m_context.frameId;

        if (m_context.frameLock[frame])
        {
            m_context.frameMutex[frame].lock();
            m_texture.Update(m_context.image[frame].data);
            m_context.frameLock[frame] = false;
            LOG(LOG_CLASSIC_TRACE, "video texture updated %d", m_context.countFrame);
            m_context.frameMutex[frame].unlock();
        }
        else
        {
            LOG(LOG_CLASSIC_TRACE, "video texture not updated \"mContext.frameLock[%d]\" is locked", m_context.frameLock[frame]);
        }

        if (IsVideoFinished() && m_isLoop)
        {
            Play();
        }
    }

    void VideoPlayer::Unload()
    {
        m_isEnabledVlC = false;

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
        if (!m_isEnabledVlC)
        {
            return false;
        }

        const libvlc_state_t state = libvlc_media_player_get_state(m_mediaPlayer);
        return state == libvlc_Ended || state == libvlc_Error;
    }

    bool VideoPlayer::IsVideoPlaying()
    {
        if (!m_isEnabledVlC)
        {
            return false;
        }

        const libvlc_state_t state = libvlc_media_player_get_state(m_mediaPlayer);
        return state == libvlc_Playing;
    }

    bool VideoPlayer::IsVideoStopped()
    {
        if (!m_isEnabledVlC)
        {
            return false;
        }

        const libvlc_state_t state = libvlc_media_player_get_state(m_mediaPlayer);
        return state == libvlc_Stopped;
    }

    void VideoPlayer::SetVolume(int volume)
    {
        if (!m_isEnabledVlC)
        {
            return;
        }

        libvlc_audio_set_volume(m_mediaPlayer, volume);
    }

} // namespace ClassicLauncher
