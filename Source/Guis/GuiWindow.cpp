#include "GuiWindow.h"
#include "Utils/UtilsFunctionLibrary.h"
#include "Utils/Resouces.h"
#include "Guis/GuiMiniCover.h"

namespace ClassicLauncher
{


    void GuiWindow::Init()
    {
        app = &Application::Get();
        width = app->GetSpecification().width;
        height = app->GetSpecification().height;
        textureName = "ref";
        
        guiGrid = app->GetEntityManager()->CreateEntity<GuiGrid>();
        miniCover = app->GetEntityManager()->CreateEntity<GuiMiniCover>();
        
        guiGrid->Init();
        miniCover->Init();

        AddChild(guiGrid.get());
		AddChild(miniCover.get());
    }


    void GuiWindow::Update()
    {
        GuiComponent::Update();

        //rootY += 1;

        if (IsKeyReleased(KEY_ENTER))
        {
            app->GetAudioManager()->PlayClick();

            if (app->GetGameListManager()->GetCurrentList() == GameListSelect)
            {
                std::string fullPath = app->GetGameListManager()->GetCurrentSystemList()->executable;
                fullPath.append(" ");
                fullPath.append(app->GetGameListManager()->GetCurrentSystemList()->arguments);
                fullPath.append(" \"");
                fullPath.append(app->GetGameListManager()->GetCurrentGameList()->path);
                fullPath.append("\" ");
                const std::string optionalWorkingDirectory = GetDirectoryPath(app->GetGameListManager()->GetCurrentSystemList()->executable.c_str());
                //platformProcess->CreateProc(fullPath, optionalWorkingDirectory);
            }
            else
            {
                ChangeGrid(GameListSelect);
            }
        }
        if (IsKeyReleased(KEY_BACKSPACE))
        {
            if (app->GetGameListManager()->GetCurrentList() == GameListSelect)
            {
                ChangeGrid(SystemListSelect);
            }
        }
    }

    void GuiWindow::ChangeGrid(const CurrentList list)
    {
        ClearCovers();
        guiGrid->SetFocus(3);
        if (list == SystemListSelect)
        {
            app->GetGameListManager()->ChangeGameToSystemList();
        }
        else
        {
            app->GetGameListManager()->ChangeSystemToGameList();
        }
        guiGrid->SetCovers();
    }

    void GuiWindow::ClearCovers()
    {
        int size = app->GetGameListManager()->GetGameListSize();
        for (int i = 0; i < size; i++)
        {
            app->GetSpriteManager()->DeleteSprite(std::to_string(i) + "_CV");
            app->GetSpriteManager()->DeleteSprite(std::to_string(i) + "_MCV");
        }
    }

} // namespace ClassicLauncher
