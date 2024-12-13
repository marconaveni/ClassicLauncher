#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "GuiComponent.h"
#include "Application.h"
#include "GuiGrid.h"
#include <memory>

namespace ClassicLauncher
{

    class Application;
    class GuiGrid;

    class GuiWindow : public GuiComponent
    {

        Application* app;
        std::shared_ptr<GuiGrid> guiGrid;

    public:
        GuiWindow() = default;
        virtual ~GuiWindow() = default;
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
    };

}

#endif // GUI_WINDOW_H