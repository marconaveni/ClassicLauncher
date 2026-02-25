#include "Application.h"

#include <string_view>

#include "Audio/AudioManager.h"
#include "Entity/TestEntity.h"
#include "Graphics/RenderScreen.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/FontManager.h"
#include "Guis/GuiWindow.h"
#include "Guis/GuiBlackScreen.h"
#include "Utils/DebugOverlay.h"
#include "Helper.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/Resources.h"
#include "Utils/String.h"
#include "Utils/TimerManager.h"
#include "Utils/Utils.h"
#include "Window/RayWindow.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{


    Application::Application(ConfigurationManager& configManager,
                             SpriteManager& spriteManager,
                             TimerManager& timerManager,
                             AudioManager& audioManager,
                             FontManager& fontManager,
                             ProcessManager& processManager)
        : m_configManager(&configManager)
        , m_spriteManager(&spriteManager)
        , m_timerManager(&timerManager)
        , m_audioManager(&audioManager)
        , m_renderEntities(&spriteManager, &configManager)
        , m_entityManager(&spriteManager, &timerManager, &m_focusManager, &fontManager)
        , m_themesManager(&m_gameListManager, &spriteManager, &m_entityManager, &configManager, &audioManager)
        , m_fontManager(&fontManager)
        , m_processManager(&processManager)
    {
        //LogLevel(m_configManager->GetClassicLogLevel(), m_configManager->GetRaylibLogLevel());
        //rlw::SetTraceLogCallback(TraceLogger);
        Resources::SetClassicLauncherDirectory();
    }

    Application::~Application()
    {
    }

    void Application::Init()
    {
        m_audioManager->Init();
        m_audioManager->LoadMusics(Resources::GetMusicDirectory(), true);
        m_gameListManager.Initialize();
        m_spriteManager->Init();
        m_themesManager.Init();

        if (m_gameListManager.GetGameListSize() > 0)
        {
            m_guiWindow = m_entityManager.CreateEntity<GuiWindow>("GuiWindow",
                                                                  &m_gameListManager,
                                                                  *m_audioManager,
                                                                  *m_processManager);
            m_guiWindow->Init();
        }
        else
        {
            LOG(LOG_CLASSIC_ERROR, "system list is empty");
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
        DebugOverlay::Update(m_audioManager, &m_gameListManager);
        UpdateLogLevel();
#endif

        m_entityManager.UpdateAll();
        m_focusManager.Update();
        m_timerManager->Update();
        m_audioManager->Update();
        ProcessUpdate();
    }
    
    void Application::ProcessUpdate()
    {
        m_status = m_processManager->GetStatus();
        switch (m_status)
        {
            case ProcessStatus::NONE: break;
            case ProcessStatus::OPEN: break;
            case ProcessStatus::RUNNING: break;
            case ProcessStatus::FAILED:
            case ProcessStatus::CLOSE:
                m_guiWindow->FadeOutScreen();
                break;
            default: break;
        }
        m_processManager->UpdateRun();
    }

    void Application::End()
    {
        m_audioManager->Unload();
        m_spriteManager->Unload();
        m_entityManager.End();
    }
    
    void Application::OnGraphicsRestore()
    {
        m_spriteManager->Init();
        m_themesManager.Init();
        m_fontManager->OnGraphicsRestore();
        m_guiWindow->UpdateCovers();
    }
    
    void Application::OnGraphicsLost()
    {
        m_fontManager->OnGraphicsLost();
    }


} // namespace ClassicLauncher