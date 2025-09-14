#include "Application.h"

#include <string_view>

#include "Graphics/RenderScreen.h"
#include "Guis/GuiWindow.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/Log.h"
#include "Utils/Resources.h"
#include "Utils/StringFunctionLibrary.h"
#include "Utils/UtilsFunctionLibrary.h"
#include "Window/RayWindow.h"
#include "rl_wrap.h"
#include "Helper.h"

namespace ClassicLauncher
{

    static Application* sInstanceApplication = nullptr;

    Application::Application(ConfigurationManager& configManager)
        : m_configManager(&configManager)
        , mRenderEntities(&this->mSpriteManager)
        , mEntityManager(&this->mSpriteManager, &this->mTimerManager)
        , mGuiWindow(nullptr)
    {
        sInstanceApplication = this;  
    }

    Application::~Application()
    {
        sInstanceApplication = nullptr;
    }

    Application& Application::Get()
    {
        return *sInstanceApplication;
    }

    GuiBlackScreen* Application::GetGuiBlackScreen()
    {
        return mGuiWindow->GetGuiBlackScreen();
    }

    void Application::Init()
    {
        // m_window = std::make_unique<RayWindow>();
        // m_renderScreen = std::make_unique<RenderScreen>();

        //m_configManager.LoadConfiguration();

        LogLevel(m_configManager->GetClassicLogLevel(), m_configManager->GetRaylibLogLevel());
        rlw::SetTraceLogCallback(TraceLogger);

        Resources::SetClassicLauncherDir();
        mGameListManager.Initialize();


        // if (m_configManager.GetVSync())
        // {
        //     m_window->SetConfigFlags(RayWindow::Flags::Vsync); // vsync only enable in fullscreen set before InitWindow
        // }

// #if _DEBUG
//         std::string_view title = "Classic Launcher [DEVMODE]";
// #else
//         std::string_view title = "Classic Launcher";
// #endif

        // m_window->Init(1280, 720, title.data());
        // m_window->SetTargetFPS(m_configManager.GetTargetFps());

        // if (m_configManager.GetFullscreen())
        // {
        //     const bool isFullscreen = m_window->ToggleFullscreen();
        //     m_configManager.SetFullscreen(isFullscreen);
        //     m_configManager.SaveConfiguration();
        // }

// #ifndef _DEBUG
//         m_window->SetExitKey(0);
// #endif

        const std::string musicDir =
            String::NormalizePath(Resources::GetClassicLauncherDir() + "musics"); // theme dir

        mThemes.Init(this);
        mThemes.LoadTheme(this);

        // mPrint.LoadFont(Resources::GetFont(), 16, 0);
        //m_renderScreen->Init(1280, 720);

        mAudioManager.Init();
        mAudioManager.LoadMusics(musicDir);
        mAudioManager.LoadCLick(Resources::GetClickAudio());
        mAudioManager.LoadCursor(Resources::GetCursorAudio());

        mSpriteManager.Init();

#ifdef _DEBUG

        // For visual reference you can upload up to four images to guide you
        const std::string refPath0 =
            String::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/ref0.png");
        const std::string refPath1 =
            String::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/ref1.png");
        const std::string refPath2 =
            String::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/ref2.png");
        const std::string refPath3 =
            String::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/ref3.png");
        mSpriteManager.LoadSprite("ref0", refPath0, 1280 * 2, 720 * 2);
        mSpriteManager.LoadSprite("ref1", refPath1, 1280 * 2, 720 * 2);
        mSpriteManager.LoadSprite("ref2", refPath2, 1280 * 2, 720 * 2);
        mSpriteManager.LoadSprite("ref3", refPath3, 1280 * 2, 720 * 2);
#endif

        // rlw::Image imgs[5] = { rlw::LoadImage(Resources::GetIcon(16).c_str()),
        //                        rlw::LoadImage(Resources::GetIcon(32).c_str()),
        //                        rlw::LoadImage(Resources::GetIcon(48).c_str()),
        //                        rlw::LoadImage(Resources::GetIcon(64).c_str()),
        //                        rlw::LoadImage(Resources::GetIcon(128).c_str()) };

        // rlw::SetWindowIcons(imgs, 5);
        
        if (mGameListManager.GetGameListSize() > 0)
        {
            mGuiWindow = mEntityManager.CreateEntity<GuiWindow>("GuiWindow");
            mGuiWindow->Init();
        }
        else
        {
            LOG(LOG_CLASSIC_ERROR, "system list is empty");
            // todo create screen not found system list
        }
        // Loop();
        
        //End();
    }
    
    void Application::CreateProcess()
    {
        GetAudioManager()->Pause();
        GetProcessManager()->CreateProc(this);
    }
    
    void Application::LoadConfigurationThemes()
    {
        LOG(LOG_CLASSIC_WARNING, "here");
        mEntityManager.SetThemeValue();
    }
    
    void Application::Loop()
    {
        //while (!m_window->ShouldClose())
        //{
            // ToggleFullscreen();
            
            // update logic
            // Update(); 
            
            // draw in texture render screen
          //  m_renderScreen->BeginRender();
          //  mRenderEntities.DrawEntities(mEntityManager.GetEntities());
          //  m_renderScreen->EndRender();
            
            // draw on window
            // Draw();
        //}
    }
    
    void Application::Draw()
    {
        // m_renderScreen->BeginRender();  // esse trecho é o begin rendersystem
        mRenderEntities.DrawEntities(mEntityManager.GetEntities());
        // m_renderScreen->EndRender();

        //rlw::BeginDrawing();   // esse trecho é o endframe rendersystem
        //rlw::ClearBackground(Color::Black);
        ////m_renderScreen->Draw();
        // mPrint.DrawMessage();
        //rlw::EndDrawing();
    }

    void Application::Update()
    {
        // Log(LOG_CLASSIC_DEBUG, TEXTBOOL(InputManager::GetInputLeftFaceLeft()));
        
        mGuiWindow->Teste();
        mEntityManager.UpdateAll();
        
        mTimerManager.Update();
        mProcessManager.StatusProcessRun(this);
        //mInputManager.UpdateInputState();
        
        #ifdef _DEBUG
        


        GameList* pSystemList = mGameListManager.GetCurrentGameList();
        PRINT(TEXT("========================================"), 2.0f, "line0", Color::Lime);
        PRINT(TEXT("Music Playing %s", mAudioManager.GetMusicName().c_str()), 2.0f, "music", Color::Lime);
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
            mAudioManager.ChangeMusic();
            PRINT(TEXT("Changed music"), 5.0f);
        }

        if (Keyboard::IsReleased(Keyboard::P) && mAudioManager.IsPlayMusic())
        {
            mAudioManager.Pause();
            PRINT(TEXT("Pause music"), 5.0f);
        }
        else if (Keyboard::IsReleased(Keyboard::P))
        {
            mAudioManager.Play();
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
        // m_renderScreen->Unload();
        GetPrint()->Unload();
        mAudioManager.Unload();
        mSpriteManager.UnloadSprites();
        mEntityManager.End();
    }

    void Application::ToggleFullscreen()
    {
        // if (Keyboard::IsReleased(Keyboard::F11) ||
        //     (Keyboard::IsDown(Keyboard::LEFT_ALT) && Keyboard::IsReleased(Keyboard::ENTER)))
        // {
        //     const bool isFullscreen = m_window->ToggleFullscreen();
        //     m_configManager.SetFullscreen(isFullscreen);
        //     m_configManager.SaveConfiguration();
        //     LOG(LOG_CLASSIC_DEBUG, TEXT("Saved is fullscreen config.ini with value %s", TEXTBOOL(isFullscreen)));
        // }
    }

} // namespace ClassicLauncher