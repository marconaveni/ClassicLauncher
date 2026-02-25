#ifndef APPLICATION_H
#define APPLICATION_H


#include "Components/FocusManager.h"
#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/RenderEntities.h"
#include "Input/InputManager.h"
#include "Themes/ThemesManager.h"
#include "Utils/ProcessManager.h"


namespace ClassicLauncher
{

    class RenderScreen;

    class GuiWindow;
    class GuiBlackScreen;
    class ProcessManager;
    class VideoPlayer;
    class InputManager;
    class ThemesManager;
    class TimerManager;
    class FocusManager;
    class RayWindow;
    class ConfigurationManager;
    class SpriteManager;
    class TimerManager;
    class AudioManager;
    class FontManager;

    class Application
    {
    public:

        Application(ConfigurationManager& configManager,
                    SpriteManager& spriteManager,
                    TimerManager& timerManager,
                    AudioManager& audioManager,
                    FontManager& fontManager,
                    ProcessManager& processManager);
        ~Application();

        void Init();
        void Update();
        void ProcessUpdate();
        void Draw();
        void End();
        void OnGraphicsRestore();
        void OnGraphicsLost();
        [[nodiscard]] ProcessStatus GetStatus() const { return m_status; }

    private:

        RenderEntities m_renderEntities;
        GameListManager m_gameListManager;
        ThemesManager m_themesManager;
        FocusManager m_focusManager;
        EntityManager m_entityManager;
        
        ProcessManager* m_processManager{nullptr};
        ConfigurationManager* m_configManager{nullptr};
        SpriteManager* m_spriteManager{nullptr};
        TimerManager* m_timerManager{nullptr};
        AudioManager* m_audioManager{nullptr};
        FontManager* m_fontManager{nullptr};

        GuiWindow* m_guiWindow{nullptr};

        ProcessStatus m_status{ProcessStatus::NONE};
    };

} // namespace ClassicLauncher

#endif
