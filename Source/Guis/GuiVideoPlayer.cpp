#include "GuiVideoPlayer.h"

namespace ClassicLauncher
{
    GuiVideoPlayer::GuiVideoPlayer()
    {
    }

    bool GuiVideoPlayer::Init(std::string path, int width, int height)
    {
        Stop();
        mPlayer = std::make_unique<VideoPlayer>();
        const bool bIsplay = mPlayer->Init(path, width, height);
        mPlayer->Play();
        return bIsplay;
    }

    void GuiVideoPlayer::Stop()
    {
        mPlayer = nullptr;
    }

    void GuiVideoPlayer::Update()
    {
        GuiComponent::Update();
        
        if(!mPlayer) return;
        
        mPlayer->Update();
    }

    void GuiVideoPlayer::Draw()
    {
        GuiComponent::Draw();
        if(!mPlayer) return;
        
        Texture2D* texture = mPlayer->GetVideoTexture();
        if(texture)
        {
            DrawTexture(*texture, 0, 0 , Color::White());
        }
    }

    void GuiVideoPlayer::End()
    {
        GuiComponent::End();
        if(!mPlayer) return;
        
        mPlayer->Unload();
        Stop();
    }

}  // namespace ClassicLauncher
