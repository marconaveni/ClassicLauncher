#include "Application.h"

#include <string_view>

#include "Audio/AudioManager.h"
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
        , m_themesManager(&m_gameListManager, &spriteManager, &m_entityManager, &configManager)
    {
    }

    Application::~Application()
    {
    }

    void Application::Init()
    {
        LogLevel(m_configManager->GetClassicLogLevel(), m_configManager->GetRaylibLogLevel());
        rlw::SetTraceLogCallback(TraceLogger);

        Resources::SetClassicLauncherDir();
        m_gameListManager.Initialize();


        m_themesManager.Init();
        m_themesManager.LoadTheme();


        const std::string LauncherDir = Resources::GetClassicLauncherDir();
        const std::string musicDir = String::NormalizePath(LauncherDir + "musics"); // theme dir
        m_audioManager->Init();
        m_audioManager->LoadMusics(musicDir, true);
        m_audioManager->LoadSound(Resources::GetClickAudio(), "click");
        m_audioManager->LoadSound(Resources::GetCursorAudio(), "cursor");

        m_spriteManager->Init();

#ifdef _DEBUG

        // For visual reference you can upload up to four images to guide you
        const std::string refPath0 = String::NormalizePath(LauncherDir + "themes/debug/ref0.png");
        const std::string refPath1 = String::NormalizePath(LauncherDir + "themes/debug/ref1.png");
        const std::string refPath2 = String::NormalizePath(LauncherDir + "themes/debug/ref2.png");
        const std::string refPath3 = String::NormalizePath(LauncherDir + "themes/debug/ref3.png");
        m_spriteManager->LoadSprite("ref0", refPath0, 1280 * 2, 720 * 2);
        m_spriteManager->LoadSprite("ref1", refPath1, 1280 * 2, 720 * 2);
        m_spriteManager->LoadSprite("ref2", refPath2, 1280 * 2, 720 * 2);
        m_spriteManager->LoadSprite("ref3", refPath3, 1280 * 2, 720 * 2);
#endif

        if (m_gameListManager.GetGameListSize() > 0)
        {
            mGuiWindow = m_entityManager.CreateEntity<GuiWindow>("GuiWindow",
                                                                 &m_gameListManager,
                                                                 *m_audioManager,
                                                                 m_processManager);
            mGuiWindow->Init();
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
        // Log(LOG_CLASSIC_DEBUG, TEXTBOOL(InputManager::GetInputLeftFaceLeft()));

        mGuiWindow->Teste();
        m_entityManager.UpdateAll();

        m_timerManager->Update();
        m_processManager.StatusProcessRun(mGuiWindow->GetGuiBlackScreen(), m_audioManager);

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

        if (InputManager::IsRelease(InputName::rightThumb, main))
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
            std::string homeDir = Utils::GetHomeDir();

            LOG(LOG_CLASSIC_DEBUG, TEXT("GetHomeDir %s", homeDir.c_str()));
            LOG(LOG_CLASSIC_DEBUG, TEXT("GetWorkingDirectory %s", Utils::GetWorkingDirectory().c_str()));
            LOG(LOG_CLASSIC_DEBUG, TEXT("GetApplicationDirectory %s", rlw::GetApplicationDirectory()));
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