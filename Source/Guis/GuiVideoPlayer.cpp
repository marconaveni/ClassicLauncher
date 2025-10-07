#include "GuiVideoPlayer.h"

#include "Entity/EntityManager.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/SpriteManager.h"
#include "Themes/ThemesManager.h"
#include "rl_wrap.h"
#include "Utils/Utils.h"
#include "Window/RayWindow.h"
#include "Input/InputManager.h"

namespace ClassicLauncher
{
    GuiVideoPlayer::GuiVideoPlayer()
        : m_player(nullptr), m_playerFullScreen(nullptr)
    {
        SetOpacity(0);
        m_gui.SetOpacity(0);
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
        VideoFadeinAnimate(1.0f, this);


        return bIsplay;
    }

    void GuiVideoPlayer::InitFullscreen()
    {
        if (!m_player || m_playerFullScreen)
        {
            return;
        }

        m_player->Pause();
        m_playerFullScreen = nullptr;
        m_playerFullScreen = std::make_unique<VideoPlayer>();
        const float scale = ThemesManager::GetScaleRenderer();
        //m_playerFullScreen->Init(m_filePath, WindowSpecs::Width, WindowSpecs::Height, scale);
        Sizei monitorSize{RayWindow::GetMonitorWidth(RayWindow::GetCurrentMonitor()), 
                             RayWindow::GetMonitorHeight(RayWindow::GetCurrentMonitor())};
        m_playerFullScreen->Init(m_filePath, monitorSize.width, monitorSize.height, scale);
        m_playerFullScreen->Play();
        m_playerFullScreen->SetLoop(false);
        GetEntityManager()->SetZOrder(this, 99); // todo temp
        VideoFadeinAnimate(0.5f, &m_gui);
        
        InputManager::SetCategory(VIDEO_FULLSCREEN);
        InputManager::RemoveCategory(MAIN);
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
        
        InputManager::SetCategory(MAIN);
        InputManager::RemoveCategory(VIDEO_FULLSCREEN);
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

        const Sizef textureSize = m_renderTexture->GetTexture()->GetSize();
        SetSource(textureSize.width, textureSize.height);
        SetSize(textureSize.width / m_renderScale, textureSize.height / m_renderScale);

        DrawVideo();

        if (!m_playerFullScreen)
        {
            return;
        }

        m_playerFullScreen->Update();
        if (m_playerFullScreen->IsVideoFinished())
        {
            StopFullscreen();
        }
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

        const Sizef sizeVideo = textureVideo->GetSize();

        RectFloat sourceRect{0, 0, sizeVideo.width, sizeVideo.height};
        RectFloat videoTransformRect{(GetSource().width - sizeVideo.width) / 2,   // aqui não é escala
                                     (GetSource().height - sizeVideo.height) / 2, //aqui não é escala
                                     sizeVideo.width,
                                     sizeVideo.height};

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
            const Color color = m_gui.GetColor();
            const float scale = ThemesManager::GetScaleRenderer();
            

            rlw::DrawTexturePro(*textureBlack,
                                RectFloat{0.0f, 0.0f, WindowSpecs::Width * scale, WindowSpecs::Height * scale},
                                RectFloat{0.0f, 0.0f, WindowSpecs::Width * scale, WindowSpecs::Height * scale},
                                Vector2f{0.0f, 0.0f},
                                0.0f,
                                color);

            Vector2f sizeVideo{
                textureFullScreen->GetSize().width * scale,
                textureFullScreen->GetSize().height * scale
            };

            textureFullScreen->SetSmooth(true);


            Utils::SetSizeWithProportionFit(sizeVideo, WindowSpecs::Width * scale, WindowSpecs::Height * scale);
            const float x = ((WindowSpecs::Width * scale) - sizeVideo.x) / 2;
            rlw::DrawTexturePro(*textureFullScreen,
                                RectFloat{0.0f, 0.0f, textureFullScreen->GetSize().width, textureFullScreen->GetSize().height},
                                RectFloat{x, 0.0f, sizeVideo.x , sizeVideo.y},
                                Vector2f{0.0f, 0.0f},
                                0.0f,
                                color);
            //rlw::DrawTexture(*textureFullScreen, x, 0, color);
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

    void GuiVideoPlayer::VideoFadeinAnimate(float time, Entity* entity)
    {
        entity->SetOpacity(0);
        Transform target = entity->GetTransform();
        target.color.a = 255;
        GetAnimationManager().StartAnimation("video-fade", time, entity, target, Ease::EaseLinearNone, false);
    }

} // namespace ClassicLauncher
