#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "Entity/EntityGui.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{

    class GuiHorizontalCards;
    class GuiBlackScreen;
    class GuiVideoPlayer;
    class GuiComponent;
    class EntityGui;

    class GuiWindow : public EntityGui
    {
    public:

        GuiWindow();
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
        void OnClick();
        void OnBack();
        void Teste();
        GuiBlackScreen* GetGuiBlackScreen() const { return mGuiBlackScreen; }

    private:

        GuiHorizontalCards* mGuiHorizontalBox;
        GuiBlackScreen* mGuiBlackScreen;
        GuiVideoPlayer* mGuiVideoPlayer;
        GuiComponent* mGuiBackground;
        TimerHandling mClickTimer;
        TimerHandling mInputTimer;
    };

} // namespace ClassicLauncher

#endif // GUI_WINDOW_H