#include "GuiWindow.h"
#include "Utils/Resources.h"
#include "Utils/UtilsFunctionLibrary.h"

namespace ClassicLauncher
{

    void GuiWindow::Init()
    {
        mApplication = &Application::Get();
        //width = mApplication->GetSpecification().width;
        //height = mApplication->GetSpecification().height;
        mProperties.width = mApplication->GetSpecification().width;
        mProperties.height = mApplication->GetSpecification().height;

        textureName = "ref";

        mGuiHorizontalBox = mApplication->GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        mGuiHorizontalBox->Init();
        AddChild(mGuiHorizontalBox.get());
    }

    void GuiWindow::Update()
    {
        GuiComponent::Update();

        // rootY += 1;

        if (IsKeyReleased(KEY_ENTER))
        {
            mApplication->GetAudioManager()->PlayClick();

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
                ChangeList(GameListSelect);
            }
        }
        if (IsKeyReleased(KEY_BACKSPACE))
        {
            if (mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
            {
                ChangeList(SystemListSelect);
            }
        }
    }

    void GuiWindow::ChangeList(const CurrentList list)
    {
        mGuiHorizontalBox->ChangeList(list);
    }

}  // namespace ClassicLauncher
