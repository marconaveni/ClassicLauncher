#include "GuiWindow.h"
#include "Utils/Resources.h"

namespace ClassicLauncher
{

    void GuiWindow::Init()
    {
        mApplication = &Application::Get();
        mProperties.width = mApplication->GetSpecification().width;
        mProperties.height = mApplication->GetSpecification().height;

        mGuiBackground = mApplication->GetEntityManager()->CreateEntity<GuiComponent>("GuiBackground");
        mGuiBackground->mProperties.sourceX = 1047;
        mGuiBackground->mProperties.sourceY = 195;
        mGuiBackground->mProperties.width = 96;
        mGuiBackground->mProperties.height = 87;
        mGuiBackground->mProperties.scaleWidth = 1280;
        mGuiBackground->mProperties.scaleHeight = 720;
        mGuiBackground->mTextureName = "sprite";
        AddChild(mGuiBackground.get());

        mGuiHorizontalBox = mApplication->GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        mGuiHorizontalBox->Init();
        AddChild(mGuiHorizontalBox.get());

        mGuiVideoPlayer = mApplication->GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");
        mGuiBlackScreen = mApplication->GetEntityManager()->CreateEntity<GuiBlackScreen>("GuiBlackScreen");
    }

    void GuiWindow::Update()
    {
        GuiComponent::Update();

        // rootY += 1;

#ifdef _DEBUG
        if (IsKeyReleased(KEY_FOUR))
        {
            mTextureName = mTextureName != "ref" ? "ref" : "transparent";
            mGuiBackground->mTextureName = mTextureName != "transparent" ? "transparent" : "sprite";
        }
#endif

        //if (InputManager::GetInputLeftFaceUp())
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
        //if (InputManager::GetInputLeftFaceDown())
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

        //if (InputManager::GetInputRightFaceDown())  // accept
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
            mApplication->GetEntityManager()->SetTimer(mTimer, CALLFUNCTION(OnClick, this), this, 0.5f, false);
        }
        if (InputManager::IsRelease(InputName::rightFaceRight) && mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)  // back
        {
            InputManager::DisableInput();
            mGuiBlackScreen->FadeInFadeOut();
            mApplication->GetEntityManager()->SetTimer(mTimer, CALLFUNCTION(OnBack, this), this, 0.5f, false);
        }
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
            InputManager::EnableInput();
        }
    }

    void GuiWindow::OnBack()
    {
        LogClassic(LOG_CLASSIC_INFO, "Called OnBack");
        if (mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
        {
            mGuiHorizontalBox->ChangeList(SystemListSelect);
            InputManager::EnableInput();
        }
    }

}  // namespace ClassicLauncher
