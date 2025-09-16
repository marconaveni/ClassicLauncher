#include "GuiWindow.h"

#include "Application.h"
#include "Entity/EntityManager.h"
#include "Guis/GuiBlackScreen.h"
#include "Guis/GuiComponent.h"
#include "Guis/GuiHorizontalCards.h"
#include "Guis/GuiVideoPlayer.h"
#include "Utils/Log.h"

namespace ClassicLauncher
{
    GuiWindow::GuiWindow(GameListManager* gameListManagerRef)
        : mGuiHorizontalBox(nullptr)
        , mGuiBlackScreen(nullptr)
        , mGuiVideoPlayer(nullptr)
        , mGuiBackground(nullptr)
        , m_gameListManagerRef(gameListManagerRef)
    {
    }

    void GuiWindow::Init()
    {
        
        mTransform.position.width = 1280.0f; //  todo: refactor    (float)pApplication->GetSpecification().width;
        mTransform.position.height = 720.0f; //  todo: refactor    (float)pApplication->GetSpecification().height;

        mGuiBackground = GetEntityManager()->CreateEntity<GuiComponent>("GuiBackground");
        mGuiBackground->mTransform.position.width = 21;
        mGuiBackground->mTransform.position.height = 720;
        mGuiBackground->mTransform.source.x = 0;
        mGuiBackground->mTransform.source.y = 562;
        mGuiBackground->mTransform.source.width = 1280;
        mGuiBackground->mTransform.source.height = 720;
        mGuiBackground->mTextureName = "sprite";
        AddChild(mGuiBackground);

        mGuiHorizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalCards>("GuiHorizontalCards", m_gameListManagerRef);
        mGuiHorizontalBox->Init();
        AddChild(mGuiHorizontalBox);

        mGuiVideoPlayer = GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");
        mGuiBlackScreen = GetEntityManager()->CreateEntity<GuiBlackScreen>("GuiBlackScreen");
        GetEntityManager()->SetZOrder(mGuiVideoPlayer, 5);
        GetEntityManager()->SetZOrder(mGuiBlackScreen, 999);

        // GetApplication()->LoadConfigurationThemes();
        GetEntityManager()->SetThemeValue();

#ifdef _DEBUG
        InputManager::SetCategory(main | debug);
#else
        InputManager::SetCategory(main);
#endif
    }

    void GuiWindow::Update()
    {
        EntityGui::Update();


#ifdef _DEBUG
        if (Keyboard::IsReleased(Keyboard::Key::ONE))
        {
            mTextureName = mTextureName != "ref0" ? "ref0" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }
        if (Keyboard::IsReleased(Keyboard::Key::TWO))
        {
            mTextureName = mTextureName != "ref1" ? "ref1" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }
        if (Keyboard::IsReleased(Keyboard::Key::THREE))
        {
            mTextureName = mTextureName != "ref2" ? "ref2" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }
        if (Keyboard::IsReleased(Keyboard::Key::FOUR))
        {
            mTextureName = mTextureName != "ref3" ? "ref3" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }

#endif

        Application* pApplication = GetApplication();

        if (InputManager::IsRelease(InputName::leftFaceUp, videoFullscreen))
        {
            if (mGuiVideoPlayer->IsPlayingFullscreen())
            {
                mGuiVideoPlayer->StopFullscreen();
                return;
            }
            mGuiVideoPlayer->Stop();
            pApplication->GetAudioManager()->Play();
            InputManager::SetCategory(main);
            InputManager::RemoveCategory(videoFullscreen);
        }

        if (InputManager::IsRelease(InputName::leftFaceDown, videoFullscreen))
        {
            if (mGuiVideoPlayer->IsPlaying())
            {
                mGuiVideoPlayer->InitFullscreen();
            }
        }
        else if (InputManager::IsRelease(InputName::leftFaceDown, main))
        {
            const bool bIsplay =
                mGuiVideoPlayer->Init(m_gameListManagerRef->GetCurrentGameList()->video, 640, 480);
            if (bIsplay)
            {
                InputManager::RemoveCategory(main);
                InputManager::SetCategory(videoFullscreen);
                pApplication->GetAudioManager()->Pause();
            }
        }
        if (InputManager::IsRelease(InputName::rightFaceLeft, main | videoFullscreen))
        {
            mGuiHorizontalBox->Click();
        }
        if (InputManager::IsRelease(InputName::rightFaceDown, main))
        {
            InputManager::DisableInput();
            pApplication->GetAudioManager()->PlayClick();
            mGuiHorizontalBox->Click();
            if (m_gameListManagerRef->GetCurrentList() == GameListSelect)
            {
                mGuiBlackScreen->FadeIn();
            }
            else
            {
                mGuiBlackScreen->FadeInFadeOut();
            }
            GetTimerManager()->SetTimer(mClickTimer, CALLFUNCTION(OnClick, this), this, 0.5f, false);
        }
        if (InputManager::IsRelease(InputName::rightFaceRight, main) &&
            m_gameListManagerRef->GetCurrentList() == GameListSelect) // back
        {
            InputManager::DisableInput();
            mGuiBlackScreen->FadeInFadeOut();
            GetTimerManager()->SetTimer(mClickTimer, CALLFUNCTION(OnBack, this), this, 0.5f, false);
        }
    }

    void GuiWindow::OnClick()
    {
        LOG(LOG_CLASSIC_INFO, "Called OnClick");
        Application* pApplication = GetApplication();
        if (m_gameListManagerRef->GetCurrentList() == GameListSelect)
        {
            pApplication->CreateProcess();
        }
        else
        {
            mGuiHorizontalBox->ChangeList(GameListSelect);
            pApplication->GetThemes()->LoadTheme();
            GetTimerManager()->SetTimer(
                mInputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::OnBack()
    {
        LOG(LOG_CLASSIC_INFO, "Called OnBack");
        Application* pApplication = GetApplication();
        if (m_gameListManagerRef->GetCurrentList() == GameListSelect)
        {
            mGuiHorizontalBox->ChangeList(SystemListSelect);
            pApplication->GetThemes()->LoadTheme();
            GetTimerManager()->SetTimer(
                mInputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::Teste()
    {
#ifdef _DEBUG
        if (Keyboard::IsReleased(Keyboard::Key::EIGHT))
        {
            if (mGuiHorizontalBox == nullptr)
            {
                mGuiHorizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalCards>("GuiHorizontalBox", m_gameListManagerRef);
                mGuiHorizontalBox->Init();
                AddChild(mGuiHorizontalBox);
                GetEntityManager()->SetThemeValue();
                InputManager::EnableInput();
                return;
            }
            mGuiHorizontalBox->SelfDelete();
            mGuiHorizontalBox = nullptr;
        }
#endif
    }
} // namespace ClassicLauncher