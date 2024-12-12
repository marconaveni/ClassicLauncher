#include "GuiWindow.h"
#include "Utils/UtilsFunctionLibrary.h"
#include "Utils/Resouces.h"

namespace ClassicLauncher
{


    void GuiWindow::Init(Application* appRef)
    {
        app = appRef;
        width = app->GetSpecification().width;
        height = app->GetSpecification().height;
        textureName = "ref";
    }

    void GuiWindow::Update()
    {
        GuiComponent::Update();
        x++;
    }

} // namespace ClassicLauncher
