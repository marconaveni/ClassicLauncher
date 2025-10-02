#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

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

    class Application
    {
    public:

        Application(ConfigurationManager& configManager,
                    SpriteManager& spriteManager,
                    TimerManager& timerManager,
                    AudioManager& audioManager);
        ~Application();

        void Init();
        void Update();
        void Draw();
        void End();

    private:

        RenderEntities m_renderEntities;
        GameListManager m_gameListManager;
        ProcessManager m_processManager;
        ThemesManager m_themesManager;
        FocusManager m_focusManager;
        EntityManager m_entityManager;

        ConfigurationManager* m_configManager = nullptr;
        SpriteManager* m_spriteManager = nullptr;
        TimerManager* m_timerManager = nullptr;
        AudioManager* m_audioManager = nullptr;

        GuiWindow* m_guiWindow = nullptr;
    };

} // namespace ClassicLauncher

#endif
