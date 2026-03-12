#ifndef ENGINE_H
#define ENGINE_H

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
#include "Window/RayWindow.h"


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
        static int GetTotalFrames();

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
        RayWindow m_window;
        RenderSystem m_renderSystem;
        InputManager m_inputManager;
        Application m_application;
        SpriteManager m_spriteManager;
        TimerManager m_timerManager;
        AudioManager m_audioManager;
        FontManager m_fontManager;
        Print m_print;
        ProcessManager m_processManager;
        std::vector<std::string> m_windowIcons{};
        EngineState m_state{EngineState::EXITING};
    };


} // namespace ClassicLauncher


#endif // ENGINE_H