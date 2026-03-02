#include "Engine.h"

#include <chrono>
#include <format>
#include <string_view>
#include <thread>

#include "ClassicLauncher.h"
#include "Helper.h"
#include "Utils/Log.h"
#include "Utils/Resources.h"
#include "Window/WindowSystem.h"

namespace ClassicLauncher
{

    Engine::Engine()
        : m_application(m_configurationManager,
                        m_spriteManager,
                        m_timerManager,
                        m_audioManager,
                        m_fontManager,
                        m_processManager)
        , m_print(m_fontManager)
    {
        RegistryPrint(&m_print);
        WindowSystem::Get().Bind(&m_window);
    }

    Engine::~Engine()
    {
        WindowSystem::Get().Bind(nullptr);
    }

    void Engine::Run()
    {
        Init();

        if (m_window.IsReady())
        {
            ChangeState(EngineState::UI_ACTIVE);
        }

        while (m_state != EngineState::EXITING)
        {
            HandleState();
        }

        m_application.End();
    }

    void Engine::Init()
    {
        m_windowIcons = {Resources::GetIconFile(16).c_str(),
                         Resources::GetIconFile(32).c_str(),
                         Resources::GetIconFile(48).c_str(),
                         Resources::GetIconFile(64).c_str(),
                         Resources::GetIconFile(128).c_str()};

        InitWindow();
        m_application.Init();
    }

    void Engine::InitWindow()
    {
        m_window.Init(WindowSpecs::Width, WindowSpecs::Height, WindowSpecs::Title.data(), m_configurationManager);
        m_window.SetIcons(m_windowIcons);
        m_renderSystem.Init(WindowSpecs::Width, WindowSpecs::Height);
        m_print.Init();
    }

    void Engine::ShutdownWindow()
    {
        m_renderSystem.Unload();
        m_spriteManager.Unload();
        m_application.OnGraphicsLost();
        m_window.Close();
    }

    void Engine::TickUi()
    {
        m_inputManager.UpdateInputState();
        m_application.Update();

        m_renderSystem.BeginFrame(); // drawing on the renderscreen
        m_application.Draw();
        m_renderSystem.EndFrame();

        m_renderSystem.BeginDraw();
        m_renderSystem.DrawRender();
        m_print.DrawMessage(); // note: Here it is drawing outside the renderscreen.
        m_renderSystem.EndDraw();

        m_window.PoolEvents();
    }

    void Engine::ProcessUpdate(int delayMs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs)); // wait
        m_application.ProcessUpdate();
    }

    void Engine::CheckProcessIsOpen()
    {
        if (m_processManager.GetStatus() == ProcessStatus::OPEN)
        {
            ChangeState(EngineState::LAUNCH_REQUESTED);
        }
    }

    void Engine::ChangeState(const EngineState newState)
    {
        m_state = newState;
    }

    void Engine::HandleState()
    {
        switch (m_state)
        {
            case EngineState::UI_ACTIVE: StateUiActive(); break;
            case EngineState::LAUNCH_REQUESTED: StateLaunchRequested(); break;
            case EngineState::PROCESS_RUNNING: StateProcessRunning(); break;
            case EngineState::SUSPENDED: StateSuspended(); break;
            case EngineState::RESTORING: StateRestoring(); break;
            case EngineState::EXITING: break;
            default: break;
        }
    }

    void Engine::StateUiActive()
    {
        if (m_window.ShouldClose())
        {
            ChangeState(EngineState::EXITING);
            return;
        }
        TickUi();
        CheckProcessIsOpen();
        ProcessUpdate(0);
    }

    void Engine::StateLaunchRequested()
    {
        m_processManager.Launch();
        if (m_processManager.GetStatus() == ProcessStatus::FAILED)
        {
            ProcessUpdate(0);
            ChangeState(EngineState::UI_ACTIVE);
            return;
        }
        ProcessUpdate(0);
        if (m_configurationManager.GetSuspendWindow())
        {
            ShutdownWindow();
            ChangeState(EngineState::SUSPENDED);
            return;
        }
        m_window.SetTargetFPS(3);
        ChangeState(EngineState::PROCESS_RUNNING);
    }

    void Engine::StateProcessRunning()
    {
        ProcessUpdate(80);
        m_renderSystem.BeginDraw();
        m_renderSystem.EndDraw();
        if (m_processManager.GetStatus() != ProcessStatus::RUNNING)
        {
            m_window.SetTargetFPS(m_configurationManager.GetTargetFps());
            ChangeState(EngineState::UI_ACTIVE);
        }
    }

    void Engine::StateSuspended()
    {
        ProcessUpdate(80);
        if (m_processManager.GetStatus() != ProcessStatus::RUNNING)
        {
            ChangeState(EngineState::RESTORING);
        }
    }

    void Engine::StateRestoring()
    {
        InitWindow();
        m_application.OnGraphicsRestore();
        m_window.SetTargetFPS(m_configurationManager.GetTargetFps());
        ChangeState(EngineState::UI_ACTIVE);
    }

} // namespace ClassicLauncher
