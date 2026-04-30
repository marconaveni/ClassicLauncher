#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "Guis/Components/GuiCanvas.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{

    class GuiHorizontalCards;
    class GuiBlackScreen;
    class GuiVideoPlayer;
    class GuiBase;
    class GameListManager;
    class AudioManager;
    class ProcessManager;

    class GuiWindow : public GuiCanvas
    {
    public:

        explicit GuiWindow(EntityContext& entityContext, GameListManager* gameListManagerRef, ProcessManager& processManagerRef);
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
        void OnClick();
        void OnBack();
        void UpdateCovers();
        void Focus();
        void FadeOutScreen();

    private:

        GuiHorizontalCards* m_guiHorizontalCards{nullptr};
        GuiBlackScreen* m_guiBlackScreen{nullptr};
        GuiVideoPlayer* m_guiVideoPlayer{nullptr};
        GuiBase* m_guiBackground{nullptr};
        TimerHandling m_clickTimer{};
        TimerHandling m_inputTimer{};

        GameListManager* m_gameListManagerRef{nullptr};
        ProcessManager* m_processManagerRef{nullptr};

#ifdef _DEBUG
        void InitDebug();
#endif
    };

} // namespace ClassicLauncher

#endif // GUI_WINDOW_H