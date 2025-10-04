#include "Application.h"

#include <string_view>

#include "Audio/AudioManager.h"
#include "Entity/TestEntity.h"
#include "Graphics/RenderScreen.h"
#include "Graphics/SpriteManager.h"
#include "Guis/GuiWindow.h"
#include "Helper.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/Log.h"
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
                             AudioManager& audioManager)
        : m_configManager(&configManager)
        , m_spriteManager(&spriteManager)
        , m_timerManager(&timerManager)
        , m_audioManager(&audioManager)
        , m_renderEntities(&spriteManager)
        , m_entityManager(&spriteManager, &timerManager, &m_focusManager)
        , m_themesManager(&m_gameListManager, &spriteManager, &m_entityManager, &configManager, &audioManager)
    {
    }
    
    Application::~Application()
    {
        LogLevel(m_configManager->GetClassicLogLevel(), m_configManager->GetRaylibLogLevel());
        rlw::SetTraceLogCallback(TraceLogger);
        Resources::SetClassicLauncherDirectory();
    }
    
    void Application::Init()
    {
        m_audioManager->Init();
        m_themesManager.Init();
              
        m_audioManager->LoadMusics(Resources::GetMusicDirectory(), true);
   
        m_spriteManager->Init();     
        m_gameListManager.Initialize();

        if (m_gameListManager.GetGameListSize() > 0)
        {
            m_guiWindow = m_entityManager.CreateEntity<GuiWindow>("GuiWindow",
                                                                 &m_gameListManager,
                                                                 *m_audioManager,
                                                                 m_processManager);
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


        if (Keyboard::IsReleased(Keyboard::F10))
        {
            Entity* parent = m_guiWindow; // Pega uma entidade qualquer como pai
            for (int i = 0; i < 5000; ++i)
            {
                // Cria 5000 entidades filhas, uma dentro da outra (hierarquia profunda)
                auto* newChild = m_entityManager.CreateEntity<GuiBase>("StressTestChild");
                newChild->SetPosition(1.0f, 1.0f); // Posição local pequena
                parent->AddChild(newChild);
                parent = newChild;
            }
            PRINT("Stress Test: 5000 entities created!", 5.0f);
        }

        if (Keyboard::IsReleased(Keyboard::C))
        {
            auto testEntity = m_entityManager.CreateEntity<TestEntity>("TestEntity");
            testEntity->InitAnim();
        }


        // Log(LOG_CLASSIC_DEBUG, TEXTBOOL(InputManager::GetInputLeftFaceLeft()));

        //mGuiWindow->Teste();
        m_entityManager.UpdateAll();
        m_focusManager.Update();

        m_timerManager->Update();
        m_processManager.StatusProcessRun(m_guiWindow->GetGuiBlackScreen(), m_audioManager);

#ifdef _DEBUG


        GameList* pSystemList = m_gameListManager.GetCurrentGameList();
        PRINT(TEXT("========================================"), 2.0f, "line0", Color::Lime);
        PRINT(TEXT("Music Playing %s", m_audioManager->GetMusicName().c_str()), 2.0f, "music", Color::Lime);
        PRINT(TEXT("========================================"), 2.0f, "line", Color::Green);
        PRINT(TEXT("%d fps", RayWindow::GetFPS()), 2.0f, "fps", Color::Green);
        PRINT(TEXT("%.6f ms", RayWindow::GetFrameTime()), 2.0f, "ms", Color::Green);
        PRINT(TEXT("========================================"), 2.0f, "line2");
        if (pSystemList)
        {
            PRINT(TEXT("Current game list %s", pSystemList->name.c_str()), 2.0f, "gameList");
        }

        if (Keyboard::IsReleased(Keyboard::F1))
        {
            LogLevel(LOG_CLASSIC_DEBUG, LOG_WARNING);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_DEBUG, LOG_WARNING");
            PRINT("Enabled LOG_CLASSIC_DEBUG, LOG_WARNING", 5.0f);
        }
        if (Keyboard::IsReleased(Keyboard::F2))
        {
            LogLevel(LOG_CLASSIC_DEBUG, LOG_ALL);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_DEBUG, LOG_ALL");
            PRINT("Enabled LOG_CLASSIC_DEBUG, LOG_ALL", 5.0f);
        }
        if (Keyboard::IsReleased(Keyboard::F3))
        {
            LogLevel(LOG_CLASSIC_ALL, LOG_ALL);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_ALL, LOG_ALL");
            PRINT("Enabled LOG_CLASSIC_ALL, LOG_ALL", 5.0f);
        }

        if (InputManager::IsRelease(InputName::rightThumb, MAIN))
        {
            m_audioManager->ChangeMusic();
            PRINT(TEXT("Changed music"), 5.0f);
        }

        if (Keyboard::IsReleased(Keyboard::P) && m_audioManager->IsPlayMusic())
        {
            m_audioManager->Pause();
            PRINT(TEXT("Pause music"), 5.0f);
        }
        else if (Keyboard::IsReleased(Keyboard::P))
        {
            m_audioManager->PlayMusic();
            PRINT(TEXT("Play music"), 5.0f);
        }
        if (Keyboard::IsReleased(Keyboard::UP))
        {
            // mEntityManager.SetZOrder(mGuiWindow.get(), 1);
            std::string homeDir = Resources::GetHomeDirectory();

        }
#endif
    }

    void Application::End()
    {
        m_audioManager->Unload();
        m_spriteManager->UnloadSprites();
        m_entityManager.End();
    }


} // namespace ClassicLauncher