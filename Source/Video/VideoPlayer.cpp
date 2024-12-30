#include "VideoPlayer.h"

namespace ClassicLauncher
{
    libvlc_instance_t* VideoPlayer::mVLC = nullptr;
    // VLC prepara para renderizar um frame de vídeo.
    void* lock(void* data, void** p_pixels)
    {
        struct VideoContext* c = (struct VideoContext*)data;
        std::unique_lock<std::mutex> lock(c->frameMutex);
        *p_pixels = c->image.data;  // Aloca o ponteiro para os pixels da imagem.
        return NULL;                // Picture identifier, not needed here.
    }

    // VLC acabou de renderizar um frame de vídeo.
    void unlock(void* data, void* id, void* const* p_pixels)
    {
        struct VideoContext* c = (struct VideoContext*)data;
        c->frameLock = true;
        c->frameMutex.unlock();
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
                printf("LibVLC initialization failure.\n");
            }
        }
    }

    VideoPlayer::VideoPlayer()
        : mContext{}, bIsEnabledVlC(false), mWidth(0), mHeight(0), bLoop(false)
    {
        StartVLCInstance();
    }

    VideoPlayer::~VideoPlayer()
    {
        Unload();
    }

    bool VideoPlayer::Init(std::string path, int width, int height)
    {
        if (path.empty())
        {
            printf("path is empty.\n");
            return false;
        }
        if (!mVLC)
        {
            printf("LibVLC not initializate.\n");
            return false;
        }

        mMedia = libvlc_media_new_path(mVLC, path.c_str());
        if (!mMedia)
        {
            printf("mMedia initialization failure.\n");
            return false;
        }
        mMediaPlayer = libvlc_media_player_new_from_media(mMedia);
        if (!mMediaPlayer)
        {
            printf("mMediaPlayer initialization failure.\n");
            return false;
        }

        mWidth = Math::Clamp(width, 0, 1280);
        mHeight = Math::Clamp(height, 0, 720);

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
        UtilsFunctionLibrary::SetSizeWithProportion(textureSize, mWidth, mHeight);
        mWidthVideo = textureSize.GetIntX();
        mHeightVideo = textureSize.GetIntY();

        mContext.image = { MemAlloc(mWidthVideo * mHeightVideo * 4),  // 4 bytes pixel (RGBA)
                           mWidthVideo,
                           mHeightVideo,
                           1,
                           PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };

        mContext.texture = LoadTextureFromImage(mContext.image);

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

        if (mContext.frameLock)
        {
            std::unique_lock<std::mutex> lock(mContext.frameMutex);
            UpdateTexture(mContext.texture, mContext.image.data);
            mContext.frameMutex.unlock();
            mContext.frameLock = false;
        }
        else
        {
            // printf("chamou update sem frame estar pronto\n");
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
        if (IsTextureValid(mContext.texture))
        {
            UnloadTexture(mContext.texture);
            mContext.texture = Texture2D();
        }
        if (IsImageValid(mContext.image))
        {
            UnloadImage(mContext.image);
            mContext.image = Image();
        }
    }

    Texture2D* VideoPlayer::GetVideoTexture()
    {
        return (IsTextureValid(mContext.texture)) ? &mContext.texture : nullptr;
    }

    Vector2 VideoPlayer::GetVideoSize()
    {
        return Vector2{ static_cast<float>(mWidth), static_cast<float>(mHeight) };
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
