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

        GuiWindow(GameListManager* gameListManagerRef, AudioManager& audioManagerRef, ProcessManager& processManagerRef);
        virtual EntityType GetType() const override { return EntityType::GuiWindowClass; }
        virtual void Update() override;
        void Init();
        void OnClick();
        void OnBack();
        void Teste();
        GuiBlackScreen* GetGuiBlackScreen() const { return m_guiBlackScreen; }

    private:

        GuiHorizontalCards* m_guiHorizontalCards{nullptr};
        GuiBlackScreen* m_guiBlackScreen{nullptr};
        GuiVideoPlayer* m_guiVideoPlayer{nullptr};
        GuiBase* m_guiBackground{nullptr};
        TimerHandling m_clickTimer{};
        TimerHandling m_inputTimer{};

        GameListManager* m_gameListManagerRef{nullptr};
        AudioManager* m_audioManagerRef{nullptr};
        ProcessManager* m_processManagerRef{nullptr};
    };

} // namespace ClassicLauncher

#endif // GUI_WINDOW_H