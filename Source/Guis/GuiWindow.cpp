#include "GuiWindow.h"
#include "Utils/UtilsFunctionLibrary.h"
#include "Utils/Resouces.h"

namespace ClassicLauncher
{


    void GuiWindow::Init()
    {
        app = &Application::Get();
        width = app->GetSpecification().width;
        height = app->GetSpecification().height;
        textureName = "ref";
        
        guiGrid = app->GetEntityManager()->CreateEntity<GuiGrid>();
        guiGrid->Init();
        AddChild(guiGrid.get());
    }

    void GuiWindow::Update()
    {
        GuiComponent::Update();
    }

} // namespace ClassicLauncher
