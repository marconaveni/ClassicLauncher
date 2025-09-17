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
    class EntityManager;
    class GameListManager;
    class AudioManager;

    class GuiWindow : public EntityGui
    {
    public:

        GuiWindow(GameListManager* gameListManagerRef, AudioManager& audioManagerRef);
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

        GameListManager* m_gameListManagerRef;
        AudioManager* m_audioManagerRef;

        
    };

} // namespace ClassicLauncher

#endif // GUI_WINDOW_H