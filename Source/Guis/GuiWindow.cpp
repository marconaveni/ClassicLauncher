#include "GuiWindow.h"
#include "Utils/Resources.h"

namespace ClassicLauncher
{

    void GuiWindow::Init()
    {
        mApplication = &Application::Get();
        mProperties.width = mApplication->GetSpecification().width;
        mProperties.height = mApplication->GetSpecification().height;

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

        if (IsKeyReleased(KEY_F1))
        {
            mTextureName = "ref";
        }
        if (IsKeyReleased(KEY_F2))
        {
            mTextureName = "transparent";
        }

        if (InputManager::GetInputLeftFaceUp())
        {
            if (mGuiVideoPlayer->IsPlayingFullscreen())
            {
                mGuiVideoPlayer->StopFullscreen();
                return;
            }
            mGuiVideoPlayer->Stop();
            mApplication->GetAudioManager()->Play();
        }
        if (InputManager::GetInputLeftFaceDown())
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

        if (InputManager::GetInputRightFaceDown())  // accept
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
        if (InputManager::GetInputRightFaceRight() && mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)  // back
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
            mApplication->GetProcessManager()->CreateProc(mApplication->GetGameListManager());
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
