#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <memory>
#include "Application.h"
#include "Core.h"
#include "GuiComponent.h"
#include "GuiHorizontalBox.h"
#include "Guis/GuiBlackScreen.h"
#include "Guis/GuiVideoPlayer.h"

namespace ClassicLauncher
{

    class Application;
    class GuiHorizontalBox;
    class GuiBlackScreen;
    class GuiVideoPlayer;

    class GuiWindow : public GuiComponent
    {
        Application* mApplication;
        std::shared_ptr<GuiHorizontalBox> mGuiHorizontalBox;
        std::shared_ptr<GuiBlackScreen> mGuiBlackScreen;
        std::shared_ptr<GuiVideoPlayer> mGuiVideoPlayer;
        Timer<GuiWindow>* mTimer = nullptr;

    public:

        GuiWindow() = default;
        virtual ~GuiWindow() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
        void OnClick();
        void OnBack();
        GuiBlackScreen* GetGuiBlackScreen() { return mGuiBlackScreen.get(); }
    };

}  // namespace ClassicLauncher

#endif  // GUI_WINDOW_H