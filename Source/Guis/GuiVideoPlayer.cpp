#include "GuiVideoPlayer.h"

#include "Entity/EntityManager.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/SpriteManager.h"
#include "Themes/ThemesManager.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{
    GuiVideoPlayer::GuiVideoPlayer()
        : m_player(nullptr), m_playerFullScreen(nullptr)
    {
        SetOpacity(0);
    }

    bool GuiVideoPlayer::Init(const std::string& path, int width, int height)
    {
        if (path.empty())
        {
            return false;
        }

        m_renderScale = ThemesManager::GetScaleRenderer();

        m_filePath = path;
        m_player = nullptr;
        m_player = std::make_unique<VideoPlayer>();
        const int widthScale = static_cast<int>(width * m_renderScale);
        const int heightScale = static_cast<int>(height * m_renderScale);
        const bool bIsplay = m_player->Init(path, widthScale, heightScale, m_renderScale, true);

        m_renderTexture = GetSpriteManager()->GetRenderTexture("videoPlayer");
        if (!m_renderTexture)
        {
            GetSpriteManager()->LoadRenderTexture("videoPlayer", width * m_renderScale, height * m_renderScale);
            m_renderTexture = GetSpriteManager()->GetRenderTexture("videoPlayer");
            m_renderTexture->SetSmooth(true);
        }
        
        m_player->Play();
        VideoFadeinAnimate(1.0f);


        return bIsplay;
    }

    void GuiVideoPlayer::InitFullscreen()
    {
        if (!m_player)
        {
            return;
        }

        m_player->Pause();
        m_playerFullScreen = nullptr;
        m_playerFullScreen = std::make_unique<VideoPlayer>();
        const float scale = ThemesManager::GetScaleRenderer();
        m_playerFullScreen->Init(m_filePath, 1280 * scale, 720 * scale, scale);
        m_playerFullScreen->Play();
        GetEntityManager()->SetZOrder(this, 99); // todo temp
        VideoFadeinAnimate(0.5f);
    }

    void GuiVideoPlayer::Stop()
    {
        m_player = nullptr;
        m_playerFullScreen = nullptr;
        SetSize(Sizef{});
    }

    void GuiVideoPlayer::StopFullscreen()
    {
        m_playerFullScreen = nullptr;

        if (!m_player)
        {
            return;
        }

        m_player->Resume();
        GetEntityManager()->SetZOrder(this, 1);
    }

    void GuiVideoPlayer::Update()
    {
        Entity::Update();
        Animatable::UpdateAnimation();

        if (!m_player)
        {
            return;
        }

        m_player->Update();

        const Vector2f textureSize = m_renderTexture->GetTexture()->GetSize().ToFloat();
        SetSource(textureSize.x, textureSize.y);
        SetSize(textureSize.x / m_renderScale, textureSize.y / m_renderScale);

        DrawVideo();

        if (!m_playerFullScreen)
        {
            return;
        }

        m_playerFullScreen->Update();
    }

    void GuiVideoPlayer::DrawVideo()
    {

        Texture* textureVideo = m_player->GetVideoTexture();
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
        RectFloat videoTransformRect{(GetSource().width - sizeVideo.x) / 2,  // aqui não é escala
                                     (GetSource().height - sizeVideo.y) / 2, //aqui não é escala
                                     sizeVideo.x,
                                     sizeVideo.y};

        rlw::DrawTexturePro(*textureVideo,
                            sourceRect,         /* RectFloat{0, 562, 21, 720}, position spritesheet */
                            videoTransformRect, /* RectFloat{0, 0, 1280, 720} posx posy width_rect  height_rect */
                            Vector2f{0, 0},
                            GetRotation(),
                            GetColor());

        rlw::EndTextureMode();
    }

    void GuiVideoPlayer::Draw()
    {
        Entity::Draw();

        if (!m_player)
        {
            return;
        }

        RectFloat sourceRect{0,
                             0,
                             (GetSource().width / m_renderScale) * m_renderScale,
                             (-GetSource().height / m_renderScale) * m_renderScale};

        rlw::DrawTexturePro(*m_renderTexture->GetTexture(),
                            sourceRect,              /* RectFloat{0, 562, 21, 720}, position spritesheet */
                            m_finalRender.transform, /* RectFloat{0, 0, 1280, 720} posx posy width_rect  height_rect */
                            m_finalRender.origin,
                            GetWorldTransform().rotation,
                            GetWorldTransform().color);

        if (!m_playerFullScreen)
        {
            return;
        }

        Texture* textureFullScreen = m_playerFullScreen->GetVideoTexture();
        Texture* textureBlack = GetSpriteManager()->GetTexture("black");
        if (textureFullScreen)
        {
            const int scale = static_cast<int>(ThemesManager::GetScaleRenderer());
            const int x = (1280 * scale / 2) - (textureFullScreen->GetSize().x / 2);
            rlw::DrawTexturePro(*textureBlack, {0, 0, 1280 * scale, 720 * scale}, {0, 0, 1280 * scale, 720 * scale},{0, 0}, 0 ,GetWorldTransform().color);
            rlw::DrawTexture(*textureFullScreen, x, 0, GetWorldTransform().color);
        }
    }

    void GuiVideoPlayer::End()
    {
        Entity::End();
        Stop();
    }

    bool GuiVideoPlayer::IsPlaying()
    {
        return (!m_player) ? false : m_player->IsVideoPlaying();
    }

    bool GuiVideoPlayer::IsPlayingFullscreen()
    {
        return (!m_playerFullScreen) ? false : m_playerFullScreen->IsVideoPlaying();
    }

    void GuiVideoPlayer::VideoFadeinAnimate(float time)
    {
        SetOpacity(0);
        Transform target = GetTransform();
        target.color.a = 255;
        GetAnimationManager().StartAnimation("video-fade", time, this, target, Ease::EaseLinearNone, false);
    }

} // namespace ClassicLauncher
