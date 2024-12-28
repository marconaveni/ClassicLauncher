#include "VideoPlayer.h"

namespace ClassicLauncher
{
    // VLC prepara para renderizar um frame de vídeo.
    void* lock(void* data, void** p_pixels)
    {
        struct Context* c = (struct Context*)data;
        *p_pixels = c->image.data;  // Aloca o ponteiro para os pixels da imagem.
        return NULL;                // Picture identifier, not needed here.
    }

    // VLC acabou de renderizar um frame de vídeo.
    void unlock(void* data, void* id, void* const* p_pixels)
    {
    }

    // VLC quer exibir um frame de vídeo.
    void display(void* data, void* id)
    {
    }

    VideoPlayer::VideoPlayer()
        : mContext{}, mVLC{}, bIsEnabledVlC(false), mWidth(0), mHeight(0), bLoop(false)
    {
    }

    VideoPlayer::~VideoPlayer()
    {
        Unload();
    }

    bool VideoPlayer::Init(std::string path, int width, int height)
    {
        if(path.empty()) return false;
        
        mWidth = Math::Clamp(width, 0, 1280);
        mHeight = Math::Clamp(height, 0, 720);

        char const* vlc_argv[] = {
            "--no-xlib",
            "--avcodec-fast",         // Reduz uso de memória ao decodificar
            "--no-video-title-show",  // Remove título do vídeo
            "--no-stats"              // Evita coleta de estatísticas

        };
        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

        mVLC.libvlc = libvlc_new(vlc_argc, vlc_argv);  // LibVLC initialization instance
        if (!mVLC.libvlc)
        {
            printf("LibVLC initialization failure.\n");
            return false;
        }

        mVLC.m = libvlc_media_new_path(mVLC.libvlc, path.c_str());
        mVLC.mp = libvlc_media_player_new_from_media(mVLC.m);
        libvlc_media_release(mVLC.m);

        ////////////////////////////////////

        unsigned track_count;
        // Get the media metadata so we can find the aspect ratio
        libvlc_media_parse(mVLC.m);
        libvlc_media_track_t** tracks;
        track_count = libvlc_media_tracks_get(mVLC.m, &tracks);

        if (track_count == 0)
        {
            return false;
        }

        for (unsigned track = 0; track < track_count; ++track)
        {
            libvlc_media_track_t* tr = tracks[track];
            if (tracks[track]->i_type == libvlc_track_video)
            {
                mVLC.width = tracks[track]->video->i_width;
                mVLC.height = tracks[track]->video->i_height;
            }
            else if (tracks[track]->i_type == libvlc_track_audio)
            {
            }
        }
        libvlc_media_tracks_release(tracks, track_count);

        /////////////////////////////////

        Vector2 textureSize((float)mVLC.width, (float)mVLC.height);
        UtilsFunctionLibrary::SetSizeWithProportion(textureSize, mWidth, mHeight);
        mVLC.width = textureSize.GetIntX();
        mVLC.height = textureSize.GetIntY();

        ////////////////////////////////////

        mContext.image = GenImageColor(mVLC.width, mVLC.height, Color::Black());
        mContext.texture = LoadTextureFromImage(mContext.image);
        libvlc_video_set_format(mVLC.mp, "RGBA", mVLC.width, mVLC.height, mVLC.width * 4);
        libvlc_video_set_callbacks(mVLC.mp, lock, unlock, display, &mContext);

        bIsEnabledVlC = mVLC.libvlc;
        return bIsEnabledVlC;
    }

    void VideoPlayer::Play()
    {
        if (!bIsEnabledVlC) return;

        libvlc_media_player_stop(mVLC.mp);
        libvlc_media_player_play(mVLC.mp);
    }

    void VideoPlayer::Update()
    {
        if (!bIsEnabledVlC) return;

        if (IsVideoPlaying())
        {
            UpdateTexture(mContext.texture, mContext.image.data);
        }

        if (IsVideoFinished())
        {
            Play();
        }
    }

    void VideoPlayer::Unload()
    {
        if (mVLC.mp)
        {
            // Desativa os callbacks de vídeo para evitar acessos futuros.
            libvlc_video_set_callbacks(mVLC.mp, nullptr, nullptr, nullptr, nullptr);

            // Para o player e remove qualquer mídia ativa.
            libvlc_media_player_stop(mVLC.mp);
            libvlc_media_player_set_media(mVLC.mp, nullptr);

            // Libera recursos do VLC Media Player.
            libvlc_media_player_release(mVLC.mp);
        }
        if (mVLC.libvlc)
        {
            // Libera recursos do libVLC.
            libvlc_release(mVLC.libvlc);
        }
        mVLC.mp = nullptr;
        mVLC.m = nullptr;
        mVLC.libvlc = nullptr;

        // if (!bIsEnabledVlC) return;

        // Libera recursos do raylib.
        if (IsTextureValid(mContext.texture))
        {
            UnloadTexture(mContext.texture);
        }
        if (IsImageValid(mContext.image))
        {
            UnloadImage(mContext.image);
        }
        bIsEnabledVlC = false;
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

        const libvlc_state_t state = libvlc_media_player_get_state(mVLC.mp);
        return state == libvlc_Ended || state == libvlc_Error;
    }

    bool VideoPlayer::IsVideoPlaying()
    {
        if (!bIsEnabledVlC) return false;

        const libvlc_state_t state = libvlc_media_player_get_state(mVLC.mp);
        return state == libvlc_Playing;
    }

    bool VideoPlayer::IsVideoStopped()
    {
        if (!bIsEnabledVlC) return false;

        const libvlc_state_t state = libvlc_media_player_get_state(mVLC.mp);
        return state == libvlc_Stopped;
    }

    void VideoPlayer::SetVolume(int volume)
    {
        if (!bIsEnabledVlC) return;

        libvlc_audio_set_volume(mVLC.mp, volume);
    }

}  // namespace ClassicLauncher
