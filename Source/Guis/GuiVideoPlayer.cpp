#include "GuiVideoPlayer.h"
#include "Application.h"

namespace ClassicLauncher
{
    GuiVideoPlayer::GuiVideoPlayer()
        : mPlayer(nullptr), mPlayerFullScreen(nullptr), mFilePath()
    {
    }

    bool GuiVideoPlayer::Init(std::string path, int width, int height)
    {
        if (path.empty())
        {
            return false;
        }
        mFilePath = path;
        mPlayer = nullptr;
        mPlayer = std::make_unique<VideoPlayer>();
        const float scale = Themes::GetScaleTexture();
        const int widthScale = static_cast<int>(width * scale);
        const int heightScale = static_cast<int>(height * scale);
        const bool bIsplay = mPlayer->Init(path, widthScale, heightScale, scale, true);
        mPlayer->Play();
        return bIsplay;
    }

    void GuiVideoPlayer::InitFullscreen()
    {
        if (!mPlayer) return;

        mPlayer->Pause();
        mPlayerFullScreen = nullptr;
        mPlayerFullScreen = std::make_unique<VideoPlayer>();
        const float scale = Themes::GetScaleTexture();
        mPlayerFullScreen->Init(mFilePath, 1280 * scale, 720 * scale, scale);
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
        EntityGui::Update();

        if (!mPlayer) return;

        mPlayer->Update();

        float scale = Themes::GetScaleTexture();
        mTransform.width = mPlayer->GetVideoSize().x / scale;
        mTransform.height = mPlayer->GetVideoSize().y / scale;

        if (!mPlayerFullScreen) return;

        mPlayerFullScreen->Update();
    }

    void GuiVideoPlayer::Draw()
    {
        EntityGui::Draw();

        if (!mPlayer) return;

        Texture2D* texture = mPlayer->GetVideoTexture();
        if (texture)
        {
            const Transform& transform = mTransform;
            DrawTexturePro(*texture, mTransform.GetSource(), mTransform.GetTransform(), Vector2{ 0, 0 }, transform.rotation, transform.color);
        }

        if (!mPlayerFullScreen) return;

        Texture2D* textureFullScreen = mPlayerFullScreen->GetVideoTexture();
        if (texture)
        {
            const float scale = Themes::GetScaleTexture();
            const float x = (1280 * scale / 2) - (textureFullScreen->width / 2);
            DrawTexture(*textureFullScreen, (int)x, 0, Color::White());
        }
    }

    void GuiVideoPlayer::End()
    {
        EntityGui::End();
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
