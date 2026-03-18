#ifndef ENGINE_H
#define ENGINE_H

#include <cstdint> // Required for uint8_t
#include <vector>

#include "Application.h"
#include "Audio/AudioManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/SpriteManager.h"
#include "Input/InputManager.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/Print.h"
#include "Utils/ProcessManager.h"
#include "Utils/TimerManager.h"
#include "Window/Window.h"


namespace ClassicLauncher
{
    enum class EngineState : std::uint8_t
    {
        UI_ACTIVE,
        LAUNCH_REQUESTED,
        PROCESS_RUNNING,
        SUSPENDED,
        RESTORING,
        EXITING
    };

    class Engine
    {
    public:

        Engine();
        ~Engine();
        void Run();

    private:

        void Init();
        void InitWindow();
        void ShutdownWindow();
        void TickUi();
        void ProcessUpdate(int delayMs);
        void CheckProcessIsOpen();
        void HandleState();
        void ChangeState(const EngineState newState);

        void StateUiActive();
        void StateLaunchRequested();
        void StateProcessRunning();
        void StateSuspended();
        void StateRestoring();

        ConfigurationManager m_configurationManager;
        Window m_window;
        RenderSystem m_renderSystem;
        InputManager m_inputManager;
        // Application m_application;
        SpriteManager m_spriteManager;
        TimerManager m_timerManager;
        AudioManager m_audioManager;
        FontManager m_fontManager;
        Print m_print;
        ProcessManager m_processManager;
        Application m_application;
        std::vector<std::string> m_windowIcons{};
        EngineState m_state{EngineState::EXITING};
    };


} // namespace ClassicLauncher


#endif // ENGINE_H