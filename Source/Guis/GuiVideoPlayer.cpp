#include "GuiVideoPlayer.h"

#include "Themes/ThemesManager.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{
    GuiVideoPlayer::GuiVideoPlayer()
        : mPlayer(nullptr), mPlayerFullScreen(nullptr)
    {
    }

    bool GuiVideoPlayer::Init(const std::string& path, int width, int height)
    {
        if (path.empty())
        {
            return false;
        }

        m_renderScale = ThemesManager::GetScaleRenderer();

        mFilePath = path;
        mPlayer = nullptr;
        mPlayer = std::make_unique<VideoPlayer>();
        const int widthScale = static_cast<int>(width * m_renderScale);
        const int heightScale = static_cast<int>(height * m_renderScale);
        const bool bIsplay = mPlayer->Init(path, widthScale, heightScale, m_renderScale, true);
        m_renderTexture.Load(width * m_renderScale, height * m_renderScale);
        m_renderTexture.SetSmooth(true);
        mPlayer->Play();
        return bIsplay;
    }

    void GuiVideoPlayer::InitFullscreen()
    {
        if (!mPlayer)
        {
            return;
        }

        // mPlayer->Pause();
        // mPlayerFullScreen = nullptr;
        // mPlayerFullScreen = std::make_unique<VideoPlayer>();
        // const float scale = ThemesManager::GetScaleTexture();
        // mPlayerFullScreen->Init(mFilePath, 1280 * scale, 720 * scale, scale);
        // mPlayerFullScreen->Play();
    }

    void GuiVideoPlayer::Stop()
    {
        mPlayer = nullptr;
        mPlayerFullScreen = nullptr;
    }

    void GuiVideoPlayer::StopFullscreen()
    {
        mPlayerFullScreen = nullptr;

        if (!mPlayer)
        {
            return;
        }

        mPlayer->Resume();
    }

    void GuiVideoPlayer::Update()
    {
        EntityGui::Update();

        if (!mPlayer)
        {
            return;
        }

        mPlayer->Update();

        m_transform.source.width = m_renderTexture.GetTexture()->GetSize().x;
        m_transform.source.height = m_renderTexture.GetTexture()->GetSize().y;
        m_transform.position.width = m_renderTexture.GetTexture()->GetSize().x / m_renderScale;
        m_transform.position.height = m_renderTexture.GetTexture()->GetSize().y / m_renderScale;

        DrawVideo();

        if (!mPlayerFullScreen)
        {
            return;
        }

        mPlayerFullScreen->Update();
    }

    void GuiVideoPlayer::DrawVideo()
    {

        Texture* textureVideo = mPlayer->GetVideoTexture();
        if (!textureVideo)
        {
            return;
        }

        rlw::BeginTextureMode(m_renderTexture);
        rlw::ClearBackground(Color::Transparent);

        Vector2f sizeVideo{
            static_cast<float>(textureVideo->GetSize().x),
            static_cast<float>(textureVideo->GetSize().y),
        };

        RectFloat sourceRect{0, 0, sizeVideo.x, sizeVideo.y};
        RectFloat finalTransformRect{(m_transform.source.width - sizeVideo.x) / 2,  // aqui não é escala
                                     (m_transform.source.height - sizeVideo.y) / 2, //aqui não é escala
                                     sizeVideo.x,
                                     sizeVideo.y};

        rlw::DrawTexturePro(*textureVideo,
                            sourceRect,         /* RectFloat{0, 562, 21, 720}, posição spritesheet */
                            finalTransformRect, /* RectFloat{0, 0, 1280, 720} posx posy tam_rect  larg_rect */
                            Vector2f{0, 0},
                            m_worldTransform.rotation,
                            m_worldTransform.color);

        rlw::EndTextureMode();
    }

    void GuiVideoPlayer::Draw()
    {
        EntityGui::Draw();

        if (!mPlayer)
        {
            return;
        }

        RectFloat sourceRect{0,
                             0,
                             (m_transform.source.width / m_renderScale) * m_renderScale,
                             (-m_transform.source.height / m_renderScale) * m_renderScale};

        RectFloat finalTransformRect{
            m_worldTransform.position.x * m_worldTransform.scale.x * m_renderScale,
            m_worldTransform.position.y * m_worldTransform.scale.y * m_renderScale,
            m_transform.position.width * m_worldTransform.scale.x * m_renderScale, // Largura base * escala final
            m_transform.position.height * m_worldTransform.scale.y * m_renderScale // Altura base * escala final
        };

        rlw::DrawTexturePro(*m_renderTexture.GetTexture(),
                            sourceRect,         /* RectFloat{0, 562, 21, 720}, posição spritesheet */
                            finalTransformRect, /* RectFloat{0, 0, 1280, 720} posx posy tam_rect  larg_rect */
                            Vector2f{0, 0},
                            m_worldTransform.rotation,
                            m_worldTransform.color);

        if (!mPlayerFullScreen)
        {
            return;
        }

        Texture* textureFullScreen = mPlayerFullScreen->GetVideoTexture();
        // if (texture)
        // {
        //     const int scale = static_cast<int>(ThemesManager::GetScaleTexture());
        //     const int x = (1280 * scale / 2) - (textureFullScreen->GetSize().x / 2);
        //     rlw::DrawTexture(*textureFullScreen, x, 0, Color::White);
        // }
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

} // namespace ClassicLauncher
