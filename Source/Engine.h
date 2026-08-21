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
#include "Utils/Log.h"
#include "Utils/Print.h"
#include "Utils/ProcessManager.h"
#include "Utils/TimerManager.h"
#include "Window/Window.h"



namespace ClassicLauncher
{
    enum class EngineState : std::uint8_t
    {
        UiActive,
        LaunchRequested,
        ProcessRunning,
        Suspended,
        Restoring,
        Exiting
    };

    class Engine
    {
    public:

        Engine();
        ~Engine();
        void Run();

    private:

        void InitRuntime();
        void ShutdownRuntime();
        void TickUi();
        void ProcessUpdate(int delayMs);
        void CheckProcessIsOpen();
        void HandleState();
        void ChangeState(const EngineState newState) { m_state = newState; }

        void StateUiActive();
        void StateLaunchRequested();
        void StateProcessRunning();
        void StateSuspended();
        void StateRestoring();

        ConfigurationManager m_configurationManager;
        Log m_log; 
        Window m_window;
        RenderSystem m_renderSystem;
        InputManager m_inputManager;
        SpriteManager m_spriteManager;
        TimerManager m_timerManager;
        AudioManager m_audioManager;
        FontManager m_fontManager;
        Print m_print;
        ProcessManager m_processManager;
        Application m_application;
        std::vector<std::string> m_windowIcons{};
        EngineState m_state{EngineState::Exiting};
    };


} // namespace ClassicLauncher


#endif // ENGINE_H