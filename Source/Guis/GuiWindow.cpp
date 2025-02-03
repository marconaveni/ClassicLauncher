#include "GuiWindow.h"
#include "Utils/Resources.h"

namespace ClassicLauncher
{

    void GuiWindow::Init()
    {
        Application* pApplication = GetApplication();
        mProperties.width = (float)pApplication->GetSpecification().width;
        mProperties.height = (float)pApplication->GetSpecification().height;

        mGuiBackground = pApplication->GetEntityManager()->CreateEntity<GuiComponent>("GuiBackground");
        mGuiBackground->mProperties.sourceX = 1047;
        mGuiBackground->mProperties.sourceY = 195;
        mGuiBackground->mProperties.width = 96;
        mGuiBackground->mProperties.height = 87;
        mGuiBackground->mProperties.scaleWidth = 1280;
        mGuiBackground->mProperties.scaleHeight = 720;
        mGuiBackground->mTextureName = "sprite";
        AddChild(mGuiBackground);

        mGuiHorizontalBox = pApplication->GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        mGuiHorizontalBox->Init();
        AddChild(mGuiHorizontalBox);

        mGuiVideoPlayer = pApplication->GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");
        mGuiBlackScreen = pApplication->GetEntityManager()->CreateEntity<GuiBlackScreen>("GuiBlackScreen");
        pApplication->GetEntityManager()->SetZOrder(mGuiVideoPlayer, 5);
        pApplication->GetEntityManager()->SetZOrder(mGuiBlackScreen, 99);
    }

    void GuiWindow::Update()
    {
        GuiComponent::Update();

        // mProperties.rootX += 1;

#ifdef _DEBUG
        if (IsKeyReleased(KEY_ONE))
        {
            mTextureName = mTextureName != "ref0" ? "ref0" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }
        if (IsKeyReleased(KEY_TWO))
        {
            mTextureName = mTextureName != "ref1" ? "ref1" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }
        if (IsKeyReleased(KEY_THREE))
        {
            mTextureName = mTextureName != "ref2" ? "ref2" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }
        if (IsKeyReleased(KEY_FOUR))
        {
            mTextureName = mTextureName != "ref3" ? "ref3" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }


#endif

        Application* pApplication = GetApplication();

        if (InputManager::IsRelease(InputName::leftFaceUp))
        {
            if (mGuiVideoPlayer->IsPlayingFullscreen())
            {
                mGuiVideoPlayer->StopFullscreen();
                return;
            }
            mGuiVideoPlayer->Stop();
            pApplication->GetAudioManager()->Play();
        }

        if (InputManager::IsRelease(InputName::leftFaceDown))
        {
            if (mGuiVideoPlayer->IsPlaying())
            {
                mGuiVideoPlayer->InitFullscreen();
                return;
            }
            const bool bIsplay = mGuiVideoPlayer->Init(pApplication->GetGameListManager()->GetCurrentGameList()->video, 640, 480);
            if (bIsplay)
            {
                pApplication->GetAudioManager()->Pause();
            }
        }
        if (InputManager::IsRelease(InputName::rightFaceLeft))
        {
            mGuiHorizontalBox->Click();
        }
        if (InputManager::IsRelease(InputName::rightFaceDown))
        {
            InputManager::DisableInput();
            pApplication->GetAudioManager()->PlayClick();
            mGuiHorizontalBox->Click();
            if (pApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
            {
                mGuiBlackScreen->FadeIn();
            }
            else
            {
                mGuiBlackScreen->FadeInFadeOut();
            }
            pApplication->GetTimerManager()->SetTimer(mClickTimer, CALLFUNCTION(OnClick, this), this, 0.5f, false);
        }
        if (InputManager::IsRelease(InputName::rightFaceRight) && pApplication->GetGameListManager()->GetCurrentList() == GameListSelect)  // back
        {
            InputManager::DisableInput();
            mGuiBlackScreen->FadeInFadeOut();
            pApplication->GetTimerManager()->SetTimer(mClickTimer, CALLFUNCTION(OnBack, this), this, 0.5f, false);
        }
    }

    void GuiWindow::OnClick()
    {
        LOG(LOG_CLASSIC_INFO, "Called OnClick");
        Application* pApplication = GetApplication();
        if (pApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
        {
            pApplication->CreateProcess();
        }
        else
        {
            mGuiHorizontalBox->ChangeList(GameListSelect);
            pApplication->GetThemes()->LoadTheme(pApplication);
            pApplication->GetTimerManager()->SetTimer(mInputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::OnBack()
    {
        LOG(LOG_CLASSIC_INFO, "Called OnBack");
        Application* pApplication = GetApplication();
        if (pApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
        {
            mGuiHorizontalBox->ChangeList(SystemListSelect);
            pApplication->GetThemes()->LoadTheme(pApplication);
            pApplication->GetTimerManager()->SetTimer(mInputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::Teste()
    {
#ifdef _DEBUG
        if (IsKeyReleased(KEY_EIGHT))
        {
            if (mGuiHorizontalBox == nullptr)
            {
                mGuiHorizontalBox = GetApplication()->GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
                mGuiHorizontalBox->Init();
                AddChild(mGuiHorizontalBox);
                InputManager::EnableInput();
                return;
            }
            mGuiHorizontalBox->SelfDelete();
            mGuiHorizontalBox = nullptr;
        }
#endif
    }

}  // namespace ClassicLauncher
