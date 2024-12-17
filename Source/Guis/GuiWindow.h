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
    class GuiMiniCover;

    class GuiWindow : public GuiComponent
    {

        Application* app;
        std::shared_ptr<GuiGrid> guiGrid;
        std::shared_ptr<GuiMiniCover> miniCover;

    public:
        GuiWindow() = default;
        virtual ~GuiWindow() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
        void ChangeGrid(const CurrentList list);
        void ClearCovers();
    };

}

#endif // GUI_WINDOW_H