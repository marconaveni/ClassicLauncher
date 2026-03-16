#include "Application.h"

#include <string_view>

#include "Audio/AudioManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/RenderScreen.h"
#include "Graphics/SpriteManager.h"
#include "Guis/GuiBlackScreen.h"
#include "Guis/GuiWindow.h"
#include "Helper.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/DebugOverlay.h"
#include "Utils/Resources.h"
#include "Utils/String.h"
#include "Utils/TimerManager.h"
#include "Utils/Utils.h"
#include "Window/Window.h"
#include "Wrap.h"


namespace ClassicLauncher
{


    Application::Application(ConfigurationManager& configManager,
                             SpriteManager& spriteManager,
                             TimerManager& timerManager,
                             AudioManager& audioManager,
                             FontManager& fontManager,
                             ProcessManager& processManager,
                             Window& window)
        : m_configManagerRef(&configManager)
        , m_spriteManagerRef(&spriteManager)
        , m_timerManagerRef(&timerManager)
        , m_audioManagerRef(&audioManager)
        , m_renderEntities(&spriteManager, &configManager, &window)
        , m_entityManager(&spriteManager, &timerManager, &m_focusManager, &fontManager, &window)
        , m_themesManager(&m_gameListManager, &spriteManager, &m_entityManager, &configManager, &audioManager)
        , m_fontManagerRef(&fontManager)
        , m_processManagerRef(&processManager)
        , m_windowRef(&window)
    {
        LogLevel(m_configManagerRef->GetClassicLogLevel(), m_configManagerRef->GetRaylibLogLevel());
        rlw::SetTraceLogCallback(TraceLogger);
    }

    Application::~Application()
    {
    }

    void Application::Init()
    {
        m_gameListManager.Initialize();
        m_spriteManagerRef->Init();
        m_themesManager.Init();

        if (m_gameListManager.GetGameListSize() > 0)
        {
            m_guiWindow = m_entityManager.CreateEntity<GuiWindow>("GuiWindow", &m_gameListManager, *m_audioManagerRef, *m_processManagerRef);
            m_guiWindow->Init();
        }
        else
        {
            LOG(LogClassicError, "system list is empty");
            // todo create screen not found system list
        }
    }

    void Application::Draw()
    {
        m_renderEntities.DrawEntities(m_entityManager.GetEntities());
    }

    void Application::Update()
    {
#ifdef _DEBUG
        DebugOverlay::Update(m_audioManagerRef, &m_gameListManager, m_windowRef);
        UpdateLogLevel();
#endif

        m_entityManager.UpdateAll();
        m_focusManager.Update();
        m_timerManagerRef->Update();
        m_audioManagerRef->Update();
    }

    void Application::ProcessUpdate()
    {
        const ProcessStatus status = m_processManagerRef->GetStatus();
        switch (status)
        {
            case ProcessStatus::None: break;
            case ProcessStatus::Open: break;
            case ProcessStatus::Running: break;
            case ProcessStatus::Failed:
            case ProcessStatus::Close: m_guiWindow->FadeOutScreen(); break;
            default: break;
        }
        m_processManagerRef->UpdateRun();
    }

    void Application::End()
    {
        m_audioManagerRef->Unload();
        m_spriteManagerRef->Unload();
        m_entityManager.End();
    }

    void Application::OnGraphicsRestore()
    {
        m_spriteManagerRef->Init();
        m_themesManager.Init();
        m_fontManagerRef->OnGraphicsRestore();
        m_guiWindow->UpdateCovers();
    }

    void Application::OnGraphicsLost()
    {
        m_fontManagerRef->OnGraphicsLost();
    }


} // namespace ClassicLauncher