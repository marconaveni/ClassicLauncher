#include "VideoPlayer.h"
#include "Utils/Log.h"

namespace ClassicLauncher
{
    libvlc_instance_t* VideoPlayer::mVLC = nullptr;

    // VLC prepara para renderizar um frame de vídeo.
    void* lock(void* data, void** p_pixels)
    {
        struct VideoContext* c = (struct VideoContext*)data;

        int frame = (c->frameId ^ 1);

        c->frameMutex[frame].lock();
        c->frameLock[frame] = false;
        *p_pixels = c->image[frame].data;  // Aloca o ponteiro para os pixels da imagem.
        return NULL;                       // Picture identifier, not needed here.
    }

    // VLC acabou de renderizar um frame de vídeo.
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

    // VLC quer exibir um frame de vídeo.
    void display(void* data, void* id)
    {
        if (data == NULL) return;

        // struct VideoContext* c = (struct VideoContext*)data;
    }

    void VideoPlayer::StartVLCInstance()
    {
        char const* vlc_argv[] = {
            "--no-xlib",
            "--quiet",                // suppress logs
            "--no-video-title-show",  // remove title
            //"--verbose=2",
            "--avcodec-fast",  // Reduz uso de memória ao decodificar
            // "--no-stats"              // Evita coleta de estatísticas

        };
        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

        if (!mVLC)
        {
            mVLC = libvlc_new(vlc_argc, vlc_argv);  // LibVLC initialization instance
            if (!mVLC)
            {
                LOG(LOG_CLASSIC_FATAL, "LibVLC initialization failure.");
            }
        }
    }

    VideoPlayer::VideoPlayer()
        : mContext{}, bIsEnabledVlC(false), mWidth(0), mHeight(0), bLoop(false)
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

    bool VideoPlayer::Init(std::string path, int width, int height, float scale, bool bFill)
    {
        LOG(LOG_CLASSIC_INFO, "Initializing video with path: %s", path.c_str());
        if (path.empty())
        {
            LOG(LOG_CLASSIC_WARNING, "path is empty.");
            return false;
        }
        if (!mVLC)
        {
            LOG(LOG_CLASSIC_ERROR, "LibVLC not initializate.");
            return false;
        }

        mMedia = libvlc_media_new_path(mVLC, path.c_str());
        if (!mMedia)
        {
            LOG(LOG_CLASSIC_ERROR, "mMedia initialization failure.");
            return false;
        }
        mMediaPlayer = libvlc_media_player_new_from_media(mMedia);
        if (!mMediaPlayer)
        {
            LOG(LOG_CLASSIC_ERROR, "mMediaPlayer initialization failure.\n");
            return false;
        }

        mWidth = Math::Clamp(width, 0, 1280 * scale);
        mHeight = Math::Clamp(height, 0, 720 * scale);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        libvlc_media_parse(mMedia);  // libvlc_media_parse_with_options() is async function
#pragma GCC diagnostic pop

        // Get the media metadata so we can find the aspect ratio
        unsigned track_count;
        libvlc_media_track_t** tracks;
        track_count = libvlc_media_tracks_get(mMedia, &tracks);

        if (track_count == 0)
        {
            return false;
        }

        for (unsigned track = 0; track < track_count; ++track)
        {
            libvlc_media_track_t* tr = tracks[track];
            if (tracks[track]->i_type == libvlc_track_video)
            {
                mWidthVideo = tracks[track]->video->i_width;
                mHeightVideo = tracks[track]->video->i_height;
            }
            else if (tracks[track]->i_type == libvlc_track_audio)
            {
            }
        }
        libvlc_media_tracks_release(tracks, track_count);

        Vector2 textureSize((float)mWidthVideo, (float)mHeightVideo);
        UtilsFunctionLibrary::SetSizeWithProportion(textureSize, mWidth, mHeight, bFill);
        mWidthVideo = textureSize.GetIntX();
        mHeightVideo = textureSize.GetIntY();

        mContext.image[0] = { MemAlloc(mWidthVideo * mHeightVideo * 4),  // 4 bytes pixel (RGBA)
                              mWidthVideo,
                              mHeightVideo,
                              1,
                              PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
        mContext.image[1] = ImageCopy(mContext.image[0]);

        texture = LoadTextureFromImage(mContext.image[0]);

        libvlc_video_set_format(mMediaPlayer, "RGBA", mWidthVideo, mHeightVideo, mWidthVideo * 4);
        libvlc_video_set_callbacks(mMediaPlayer, lock, unlock, display, &mContext);

        bIsEnabledVlC = mVLC && mMedia && mMediaPlayer;
        return bIsEnabledVlC;
    }

    void VideoPlayer::Play()
    {
        if (!bIsEnabledVlC) return;

        libvlc_media_player_stop(mMediaPlayer);
        libvlc_media_player_play(mMediaPlayer);
    }

    void VideoPlayer::Pause()
    {
        if (!bIsEnabledVlC) return;

        libvlc_media_player_pause(mMediaPlayer);
    }

    void VideoPlayer::Resume()
    {
        if (!bIsEnabledVlC) return;

        libvlc_media_player_play(mMediaPlayer);
    }

    void VideoPlayer::Stop()
    {
        if (!bIsEnabledVlC) return;

        libvlc_media_player_stop(mMediaPlayer);
    }

    void VideoPlayer::Update()
    {
        if (!bIsEnabledVlC) return;

        int frame = mContext.frameId;

        if (mContext.frameLock[frame])
        {
            mContext.frameMutex[frame].lock();
            UpdateTexture(texture, mContext.image[frame].data);
            mContext.frameLock[frame] = false;
            LOG(LOG_CLASSIC_TRACE, "video texture updated %d", mContext.countFrame);
            mContext.frameMutex[frame].unlock();
        }
        else
        {
            LOG(LOG_CLASSIC_TRACE, "video texture not updated \"mContext.frameLock[%d]\" is locked", mContext.frameLock[frame]);
        }

        if (IsVideoFinished())
        {
            Play();
        }
    }

    void VideoPlayer::Unload()
    {
        bIsEnabledVlC = false;

        // Release the media player
        if (mMediaPlayer)
        {
            libvlc_media_player_stop(mMediaPlayer);
            libvlc_media_player_release(mMediaPlayer);
            mMediaPlayer = NULL;
        }

        // Release the media
        if (mMedia)
        {
            libvlc_media_release(mMedia);
            mMedia = NULL;
        }

        // Release raylib resources
        if (IsTextureValid(texture))
        {
            UnloadTexture(texture);
            texture = Texture2D();
        }
        if (IsImageValid(mContext.image[0]))
        {
            UnloadImage(mContext.image[0]);
            mContext.image[0] = Image();
        }
        if (IsImageValid(mContext.image[1]))
        {
            UnloadImage(mContext.image[1]);
            mContext.image[1] = Image();
        }
    }

    Texture2D* VideoPlayer::GetVideoTexture()
    {
        return (IsTextureValid(texture)) ? &texture : nullptr;
    }

    Vector2 VideoPlayer::GetVideoSize()
    {
        return (IsTextureValid(texture)) ? Vector2{ static_cast<float>(texture.width), static_cast<float>(texture.height) } : Vector2{ 0, 0 };
        // return Vector2{ static_cast<float>(mWidth), static_cast<float>(mHeight) };
    }

    bool VideoPlayer::IsVideoFinished()
    {
        if (!bIsEnabledVlC) return false;

        const libvlc_state_t state = libvlc_media_player_get_state(mMediaPlayer);
        return state == libvlc_Ended || state == libvlc_Error;
    }

    bool VideoPlayer::IsVideoPlaying()
    {
        if (!bIsEnabledVlC) return false;

        const libvlc_state_t state = libvlc_media_player_get_state(mMediaPlayer);
        return state == libvlc_Playing;
    }

    bool VideoPlayer::IsVideoStopped()
    {
        if (!bIsEnabledVlC) return false;

        const libvlc_state_t state = libvlc_media_player_get_state(mMediaPlayer);
        return state == libvlc_Stopped;
    }

    void VideoPlayer::SetVolume(int volume)
    {
        if (!bIsEnabledVlC) return;

        libvlc_audio_set_volume(mMediaPlayer, volume);
    }

}  // namespace ClassicLauncher
