#include "GuiVideoPlayer.h"

namespace ClassicLauncher
{
    GuiVideoPlayer::GuiVideoPlayer()
        : mFilePath()
    {
    }

    bool GuiVideoPlayer::Init(std::string path, int width, int height)
    {
        mFilePath = path;
        mPlayer = nullptr;
        mPlayer = std::make_unique<VideoPlayer>();
        const bool bIsplay = mPlayer->Init(path, width, height);
        mPlayer->Play();
        return bIsplay;
    }

    void GuiVideoPlayer::InitFullscreen()
    {
        if (!mPlayer) return;

        mPlayer->Pause();
        mPlayerFullScreen = nullptr;
        mPlayerFullScreen = std::make_unique<VideoPlayer>();
        mPlayerFullScreen->Init(mFilePath, 1280, 720);
        mPlayerFullScreen->Play();
    }

    void GuiVideoPlayer::Stop()
    {
        mPlayer = nullptr;
        mPlayerFullScreen = nullptr;
    }

    void GuiVideoPlayer::StopFullscreen()
    {
        mPlayerFullScreen = nullptr;
        
        if (!mPlayer) return;

        mPlayer->Resume();
    }

    void GuiVideoPlayer::Update()
    {
        GuiComponent::Update();

        if (!mPlayer) return;

        mPlayer->Update();

        if (!mPlayerFullScreen) return;

        mPlayerFullScreen->Update();
    }

    void GuiVideoPlayer::Draw()
    {
        GuiComponent::Draw();

        if (!mPlayer) return;

        Texture2D* texture = mPlayer->GetVideoTexture();
        if (texture)
        {
            DrawTexture(*texture, 0, 0, Color::White());
        }

        if (!mPlayerFullScreen) return;

        Texture2D* textureFullScreen = mPlayerFullScreen->GetVideoTexture();
        if (texture)
        {
            const int x = (1280 / 2) - (textureFullScreen->width / 2);
            DrawTexture(*textureFullScreen, x, 0, Color::White());
        }
    }

    void GuiVideoPlayer::End()
    {
        GuiComponent::End();
        Stop();
    }

    bool GuiVideoPlayer::IsPlaying()
    {    
        return (!mPlayer) ? false : mPlayer->IsVideoPlaying();
    }

    bool GuiVideoPlayer::IsPlayingFullscreen()
    {
        return (!mPlayerFullScreen) ? false : mPlayerFullScreen->IsVideoPlaying();
    }

}  // namespace ClassicLauncher
