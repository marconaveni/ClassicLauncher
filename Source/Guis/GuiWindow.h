#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "Core.h"
#include "GuiComponent.h"
#include "Guis/GuiHorizontalCards.h"
#include "Guis/GuiBlackScreen.h"
#include "Guis/GuiVideoPlayer.h"

namespace ClassicLauncher
{

    class GuiHorizontalCards;
    class GuiBlackScreen;
    class GuiVideoPlayer;
    class GuiComponent;

    class GuiWindow : public GuiComponent
    {
        GuiHorizontalCards* mGuiHorizontalBox;
        GuiBlackScreen* mGuiBlackScreen;
        GuiVideoPlayer* mGuiVideoPlayer;
        GuiComponent* mGuiBackground;
        TimerHandling mClickTimer;
        TimerHandling mInputTimer;

    public:

        GuiWindow();
        virtual ~GuiWindow() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
        void OnClick();
        void OnBack();
        void Teste();
        GuiBlackScreen* GetGuiBlackScreen() { return mGuiBlackScreen; }
    };

}  // namespace ClassicLauncher

#endif  // GUI_WINDOW_H