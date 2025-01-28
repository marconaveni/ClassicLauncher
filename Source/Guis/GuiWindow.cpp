#include "GuiWindow.h"
#include "Utils/Resources.h"

namespace ClassicLauncher
{

    void GuiWindow::Init()
    {
        mApplication = &Application::Get();
        mProperties.width = (float)mApplication->GetSpecification().width;
        mProperties.height = (float)mApplication->GetSpecification().height;

        mGuiBackground = mApplication->GetEntityManager()->CreateEntity<GuiComponent>("GuiBackground");
        mGuiBackground->mProperties.sourceX = 1047;
        mGuiBackground->mProperties.sourceY = 195;
        mGuiBackground->mProperties.width = 96;
        mGuiBackground->mProperties.height = 87;
        mGuiBackground->mProperties.scaleWidth = 1280;
        mGuiBackground->mProperties.scaleHeight = 720;
        mGuiBackground->mTextureName = "sprite";
        AddChild(mGuiBackground);

        mGuiHorizontalBox = mApplication->GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        mGuiHorizontalBox->Init();
        AddChild(mGuiHorizontalBox);

        mGuiVideoPlayer = mApplication->GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");
        mGuiBlackScreen = mApplication->GetEntityManager()->CreateEntity<GuiBlackScreen>("GuiBlackScreen");
       // mApplication->GetEntityManager()->SetZOrder(mGuiBlackScreen, 99);
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

        if (IsKeyReleased(KEY_NINE))
        {
           
            
        }

#endif

        if (InputManager::IsRelease(InputName::leftFaceUp))
        {
            if (mGuiVideoPlayer->IsPlayingFullscreen())
            {
                mGuiVideoPlayer->StopFullscreen();
                return;
            }
            mGuiVideoPlayer->Stop();
            mApplication->GetAudioManager()->Play();
        }

        if (InputManager::IsRelease(InputName::leftFaceDown))
        {
            if (mGuiVideoPlayer->IsPlaying())
            {
                mGuiVideoPlayer->InitFullscreen();
                return;
            }
            const bool bIsplay = mGuiVideoPlayer->Init(mApplication->GetGameListManager()->GetCurrentGameList()->video, 640, 480);
            if (bIsplay)
            {
                mApplication->GetAudioManager()->Pause();
            }
        }
        if (InputManager::IsRelease(InputName::rightFaceLeft))
        {
            mGuiHorizontalBox->Click();
        }
        if (InputManager::IsRelease(InputName::rightFaceDown))
        {
            InputManager::DisableInput();
            mApplication->GetAudioManager()->PlayClick();
            mGuiHorizontalBox->Click();
            if (mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
            {
                mGuiBlackScreen->FadeIn();
            }
            else
            {
                mGuiBlackScreen->FadeInFadeOut();
            }
            mApplication->GetEntityManager()->SetTimer(mClickTimer, CALLFUNCTION(OnClick, this), this, 0.5f, false);
        }
        if (InputManager::IsRelease(InputName::rightFaceRight) && mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)  // back
        {
            InputManager::DisableInput();
            mGuiBlackScreen->FadeInFadeOut();
            mApplication->GetEntityManager()->SetTimer(mClickTimer, CALLFUNCTION(OnBack, this), this, 0.5f, false);
        }
    }

    void GuiWindow::End()
    {
        GuiHorizontalBox* mGuiHorizontalBox = nullptr;
        GuiBlackScreen* mGuiBlackScreen = nullptr;
        GuiVideoPlayer* mGuiVideoPlayer = nullptr;
        GuiComponent* mGuiBackground = nullptr;
    }

    void GuiWindow::OnClick()
    {
        LogClassic(LOG_CLASSIC_INFO, "Called OnClick");
        if (mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
        {
            mApplication->CreateProcess();
        }
        else
        {
            mGuiHorizontalBox->ChangeList(GameListSelect);
            mApplication->GetThemes()->LoadTheme(mApplication);
            mApplication->GetEntityManager()->SetTimer(mInputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::OnBack()
    {
        LogClassic(LOG_CLASSIC_INFO, "Called OnBack");
        if (mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
        {
            mGuiHorizontalBox->ChangeList(SystemListSelect);
            mApplication->GetThemes()->LoadTheme(mApplication);
            mApplication->GetEntityManager()->SetTimer(mInputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::Teste()
    {
        if (IsKeyReleased(KEY_EIGHT))
        {
            mGuiHorizontalBox->SelfDelete();
            // mGuiHorizontalBox.reset();
            mGuiHorizontalBox = mApplication->GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
            if (mGuiHorizontalBox)
            {
                mGuiHorizontalBox->Init();
                AddChild(mGuiHorizontalBox);
            }
        }
    }

}  // namespace ClassicLauncher
