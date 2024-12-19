#include "GuiWindow.h"
#include "Guis/GuiMiniCover.h"
#include "Utils/Resouces.h"
#include "Utils/UtilsFunctionLibrary.h"

namespace ClassicLauncher
{

    void GuiWindow::Init()
    {
        mApplication = &Application::Get();
        width = mApplication->GetSpecification().width;
        height = mApplication->GetSpecification().height;
        textureName = "ref";

        mGuiHorizontalBox = mApplication->GetEntityManager()->CreateEntity<GuiHorizontalBox>();
        mMiniCover = mApplication->GetEntityManager()->CreateEntity<GuiMiniCover>();

        mGuiHorizontalBox->Init();
        mMiniCover->Init();

        AddChild(mGuiHorizontalBox.get());
        AddChild(mMiniCover.get());
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
                ChangeGrid(GameListSelect);
            }
        }
        if (IsKeyReleased(KEY_BACKSPACE))
        {
            if (mApplication->GetGameListManager()->GetCurrentList() == GameListSelect)
            {
                ChangeGrid(SystemListSelect);
            }
        }
    }

    void GuiWindow::ChangeGrid(const CurrentList list)
    {
        ClearCovers();
        mGuiHorizontalBox->SetFocus(3);
        if (list == SystemListSelect)
        {
            mApplication->GetGameListManager()->ChangeGameToSystemList();
        }
        else
        {
            mApplication->GetGameListManager()->ChangeSystemToGameList();
        }
        mGuiHorizontalBox->SetCovers();
    }

    void GuiWindow::ClearCovers()
    {
        int size = mApplication->GetGameListManager()->GetGameListSize();
        for (int i = 0; i < size; i++)
        {
            mApplication->GetSpriteManager()->DeleteSprite(std::to_string(i) + "_CV");
            mApplication->GetSpriteManager()->DeleteSprite(std::to_string(i) + "_MCV");
        }
    }

}  // namespace ClassicLauncher
