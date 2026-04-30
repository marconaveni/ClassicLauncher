#include "Engine.h"

#include <chrono>
#include <string_view>
#include <thread>

#include "ClassicLauncher.h"
#include "Helper.h"
#include "Input/Gamepad.h"
#include "Utils/Resources.h"
#include "Video/VideoPlayer.h"


namespace ClassicLauncher
{

    Engine::Engine()
        : m_log(&m_print)
        , m_application(m_configurationManager, m_spriteManager, m_timerManager, m_audioManager, m_fontManager, m_processManager, m_window)
        , m_print(m_fontManager, &m_window)
        , m_timerManager(&m_window)
        , m_renderSystem(&m_window)
    {
        m_log.EnableLogFile(m_configurationManager.GetEnableLog());
        m_log.SetLevel(m_configurationManager.GetClassicLogLevel(), m_configurationManager.GetRaylibLogLevel());
        Resources::SetClassicLauncherDirectory();
        m_windowIcons = {Resources::GetIconFile(16).c_str(),
                         Resources::GetIconFile(32).c_str(),
                         Resources::GetIconFile(48).c_str(),
                         Resources::GetIconFile(64).c_str(),
                         Resources::GetIconFile(128).c_str()};
        VideoPlayer::StartVLCInstance();
    }

    Engine::~Engine() = default;

    void Engine::Run()
    {
        InitRuntime();

        m_application.Init();
        m_audioManager.ChangeMusic();

        if (m_window.IsReady())
        {
            ChangeState(EngineState::UiActive);
        }

        while (m_state != EngineState::Exiting)
        {
            GamePad::Update();
            HandleState();
            m_log.UpdateLog();
        }

        ShutdownRuntime();
    }

    void Engine::InitRuntime()
    {
        GamePad::Init();
        m_window.Init(WindowSpecs::Title.data(), m_configurationManager);
        m_window.SetIcons(m_windowIcons);
        m_renderSystem.Init(WindowSpecs::Width, WindowSpecs::Height);
        m_print.Init();
        m_audioManager.Init();
        m_audioManager.LoadMusics(Resources::GetMusicDirectory(), false);
    }

    void Engine::ShutdownRuntime()
    {
        GamePad::Shutdown();
        m_renderSystem.Unload();
        m_spriteManager.Unload();
        m_fontManager.Suspend();
        m_window.CloseWindow();
        m_audioManager.Unload();
    }

    void Engine::TickUi()
    {
        m_inputManager.UpdateInputState(m_window.GetFrameTime());
        m_application.Update();

        m_renderSystem.BeginFrame(); // drawing on the renderscreen
        m_application.Draw();
        m_renderSystem.EndFrame();

        m_renderSystem.BeginDraw();
        m_renderSystem.DrawRender();
        m_print.DrawMessage(); // note: Here it is drawing outside the renderscreen.
        m_renderSystem.EndDraw();

        m_window.Update();
    }

    void Engine::ProcessUpdate(int delayMs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs)); // wait
        m_application.ProcessUpdate();
    }

    void Engine::CheckProcessIsOpen()
    {
        if (m_processManager.GetStatus() == ProcessStatus::Open)
        {
            ChangeState(EngineState::LaunchRequested);
        }
    }

    void Engine::HandleState()
    {
        switch (m_state)
        {
            case EngineState::UiActive: StateUiActive(); break;
            case EngineState::LaunchRequested: StateLaunchRequested(); break;
            case EngineState::ProcessRunning: StateProcessRunning(); break;
            case EngineState::Suspended: StateSuspended(); break;
            case EngineState::Restoring: StateRestoring(); break;
            case EngineState::Exiting: break;
            default: break;
        }
    }

    void Engine::StateUiActive()
    {
        if (m_window.ShouldClose())
        {
            ChangeState(EngineState::Exiting);
            return;
        }
        TickUi();
        CheckProcessIsOpen();
        ProcessUpdate(0);
    }

    void Engine::StateLaunchRequested()
    {
        m_processManager.Launch();
        if (m_processManager.GetStatus() == ProcessStatus::Failed)
        {
            ProcessUpdate(0);
            ChangeState(EngineState::UiActive);
            return;
        }
        ProcessUpdate(0);
        if (m_configurationManager.GetSuspendWindow())
        {
            ShutdownRuntime();
            ChangeState(EngineState::Suspended);
            return;
        }
        m_window.SetTargetFPS(3);
        ChangeState(EngineState::ProcessRunning);
    }

    void Engine::StateProcessRunning()
    {
        ProcessUpdate(80);
        m_renderSystem.BeginDraw();
        m_renderSystem.EndDraw();
        if (m_processManager.GetStatus() != ProcessStatus::Running)
        {
            m_window.SetTargetFPS(m_configurationManager.GetTargetFps());
            ChangeState(EngineState::UiActive);
        }
    }

    void Engine::StateSuspended()
    {
        ProcessUpdate(80);
        if (m_processManager.GetStatus() != ProcessStatus::Running)
        {
            ChangeState(EngineState::Restoring);
        }
    }

    void Engine::StateRestoring()
    {
        InitRuntime();
        m_spriteManager.Init();
        m_application.Restore();
        m_fontManager.Restore();
        m_window.SetTargetFPS(m_configurationManager.GetTargetFps());
        ChangeState(EngineState::UiActive);
    }


} // namespace ClassicLauncher
