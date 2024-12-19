#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <memory>
#include "Application.h"
#include "GuiComponent.h"
#include "GuiGrid.h"

namespace ClassicLauncher
{

    class Application;
    class GuiGrid;
    class GuiMiniCover;

    class GuiWindow : public GuiComponent
    {

        Application* mApplication;
        std::shared_ptr<GuiGrid> mGuiGrid;
        std::shared_ptr<GuiMiniCover> mMiniCover;

    public:

        GuiWindow() = default;
        virtual ~GuiWindow() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
        void ChangeGrid(const CurrentList list);
        void ClearCovers();
    };

}  // namespace ClassicLauncher

#endif  // GUI_WINDOW_H