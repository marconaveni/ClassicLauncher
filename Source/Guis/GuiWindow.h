#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <memory>
#include "Application.h"
#include "GuiComponent.h"
#include "GuiHorizontalBox.h"

namespace ClassicLauncher
{

    class Application;
    class GuiHorizontalBox;

    class GuiWindow : public GuiComponent
    {
        Application* mApplication;
        std::shared_ptr<GuiHorizontalBox> mGuiHorizontalBox;

    public:

        GuiWindow() = default;
        virtual ~GuiWindow() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
        void ChangeList(const CurrentList list);
    };

}  // namespace ClassicLauncher

#endif  // GUI_WINDOW_H