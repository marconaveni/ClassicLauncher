#include "GuiVideoPlayer.h"

#include "Themes/ThemesManager.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/SpriteManager.h"
#include "Entity/EntityManager.h"
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

        GetSpriteManager()->LoadRenderTexture("videoPlayer", width * m_renderScale, height * m_renderScale);
        m_renderTexture = GetSpriteManager()->GetRenderTexture("videoPlayer");
        m_renderTexture->SetSmooth(true);
        // m_renderTexture.Load(width * m_renderScale, height * m_renderScale);
        mPlayer->Play();
        return bIsplay;
    }

    void GuiVideoPlayer::InitFullscreen()
    {
        if (!mPlayer)
        {
            return;
        }

        mPlayer->Pause();
        mPlayerFullScreen = nullptr;
        mPlayerFullScreen = std::make_unique<VideoPlayer>();
        const float scale = ThemesManager::GetScaleRenderer();
        mPlayerFullScreen->Init(mFilePath, 1280 * scale, 720 * scale, scale);
        mPlayerFullScreen->Play();
        GetEntityManager()->SetZOrder(this, 99);  // todo temp
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
        GetEntityManager()->SetZOrder(this, 1);
    }

    void GuiVideoPlayer::Update()
    {
        Entity::Update();

        if (!mPlayer)
        {
            return;
        }

        mPlayer->Update();

        m_transform.source.width = m_renderTexture->GetTexture()->GetSize().x;
        m_transform.source.height = m_renderTexture->GetTexture()->GetSize().y;
        m_transform.position.width = m_renderTexture->GetTexture()->GetSize().x / m_renderScale;
        m_transform.position.height = m_renderTexture->GetTexture()->GetSize().y / m_renderScale;

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

        rlw::BeginTextureMode(*m_renderTexture);
        rlw::ClearBackground(Color::Transparent);

        Vector2f sizeVideo{
            static_cast<float>(textureVideo->GetSize().x),
            static_cast<float>(textureVideo->GetSize().y),
        };

        RectFloat sourceRect{0, 0, sizeVideo.x, sizeVideo.y};
        RectFloat videoTransformRect{(m_transform.source.width - sizeVideo.x) / 2,  // aqui não é escala
                                     (m_transform.source.height - sizeVideo.y) / 2, //aqui não é escala
                                     sizeVideo.x,
                                     sizeVideo.y};

        rlw::DrawTexturePro(*textureVideo,
                            sourceRect,         /* RectFloat{0, 562, 21, 720}, posição spritesheet */
                            videoTransformRect, /* RectFloat{0, 0, 1280, 720} posx posy tam_rect  larg_rect */
                            Vector2f{0, 0},
                            m_worldTransform.rotation,
                            m_worldTransform.color);

        rlw::EndTextureMode();
    }

    void GuiVideoPlayer::Draw()
    {
        Entity::Draw();

        if (!mPlayer)
        {
            return;
        }

        RectFloat sourceRect{0,
                             0,
                             (m_transform.source.width / m_renderScale) * m_renderScale,
                             (-m_transform.source.height / m_renderScale) * m_renderScale};

        rlw::DrawTexturePro(*m_renderTexture->GetTexture(),
                            sourceRect,         /* RectFloat{0, 562, 21, 720}, posição spritesheet */
                            m_finalRender.transform, /* RectFloat{0, 0, 1280, 720} posx posy tam_rect  larg_rect */
                            m_finalRender.origin,
                            m_worldTransform.rotation,
                            m_worldTransform.color);

        if (!mPlayerFullScreen)
        {
            return;
        }

        Texture* textureFullScreen = mPlayerFullScreen->GetVideoTexture();
        if (textureFullScreen)
        {
            const int scale = static_cast<int>(ThemesManager::GetScaleRenderer());
            const int x = (1280 * scale / 2) - (textureFullScreen->GetSize().x / 2);
            rlw::DrawTexture(*textureFullScreen, x, 0, Color::White);
        }
    }

    void GuiVideoPlayer::End()
    {
        Entity::End();
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
