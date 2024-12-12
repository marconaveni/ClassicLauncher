#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "GuiComponent.h"
#include "Application.h"

namespace ClassicLauncher
{

    class Application;

    class GuiWindow : public GuiComponent
    {

        Application* app;

    public:
        GuiWindow() = default;
        virtual ~GuiWindow() = default;
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init(Application* appRef);
    };

}

#endif // GUI_WINDOW_H