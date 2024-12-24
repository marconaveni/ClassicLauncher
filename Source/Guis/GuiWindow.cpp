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
        mGuiBlackScreen = mApplication->GetEntityManager()->CreateEntity<GuiBlackScreen>("GuiBlackScreen");
    }

    void GuiWindow::Update()
    {
        GuiComponent::Update();

        // rootY += 1;
        if (IsKeyReleased(KEY_ONE))
        {
            mTextureName = "ref";
        }
        if (IsKeyReleased(KEY_TWO))
        {
            mTextureName = "transparent";
        }

        if (IsKeyReleased(KEY_ENTER))
        {
            mApplication->GetAudioManager()->PlayClick();
            mGuiHorizontalBox->Click();
            mGuiBlackScreen->FadeInFadeOut();
            mApplication->GetEntityManager()->SetTimer(mTimer, &GuiWindow::OnClick, this, 0.5f, false);
        }
        if (IsKeyReleased(KEY_BACKSPACE))
        {
            mGuiBlackScreen->FadeInFadeOut();
            mApplication->GetEntityManager()->SetTimer(mTimer, &GuiWindow::OnBack, this, 0.5f, false);
        }
    }

    void GuiWindow::OnClick()
    {
        TraceLog(LOG_INFO, "Click");
        if (mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
        {
            std::string fullPath = mApplication->GetGameListManager()->GetCurrentSystemList()->executable;
            fullPath.append(" ");
            fullPath.append(mApplication->GetGameListManager()->GetCurrentSystemList()->arguments);
            fullPath.append(" \"");
            fullPath.append(mApplication->GetGameListManager()->GetCurrentGameList()->path);
            fullPath.append("\" ");
            const std::string optionalWorkingDirectory = GetDirectoryPath(mApplication->GetGameListManager()->GetCurrentSystemList()->executable.c_str());
            // platformProcess->CreateProc(fullPath, optionalWorkingDirectory);
        }
        else
        {
            mGuiHorizontalBox->ChangeList(GameListSelect);
        }
        
    }

    void GuiWindow::OnBack()
    {
        if (mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
        {
            mGuiHorizontalBox->ChangeList(SystemListSelect);
        }
    }

}  // namespace ClassicLauncher
