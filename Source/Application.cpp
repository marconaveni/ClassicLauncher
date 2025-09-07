#include "Application.h"
#include "Guis/GuiWindow.h"
#include "Helper.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/Log.h"
#include "Utils/Resources.h"
#include "Utils/StringFunctionLibrary.h"
#include "Utils/UtilsFunctionLibrary.h"

namespace ClassicLauncher
{

    static Application* sInstanceApplication = nullptr;

    Application::Application()
        : mRenderEntities(&this->mSpriteManager), mEntityManager(&this->mSpriteManager, &this->mTimerManager), mGuiWindow(nullptr)
    {
        if (sInstanceApplication == nullptr)
        {
            sInstanceApplication = this;
        }
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
        mConfigurationManager.LoadConfiguration();

        LogLevel(mConfigurationManager.GetClassicLogLevel(), mConfigurationManager.GetRaylibLogLevel());
        rlw::SetTraceLogCallback(TraceLogger);

        Resources::SetClassicLauncherDir();
        mGameListManager.Initialize();

        rlw::InitAudioDevice();

        if (mConfigurationManager.GetVSync())
        {
            rlw::SetConfigFlags(rlw::FLAG_VSYNC_HINT);  // vsync only enable in fullscreen set before InitWindow
        }
        rlw::InitWindow(mSpecification.width, mSpecification.height, mSpecification.title);
        rlw::SetWindowState(rlw::FLAG_WINDOW_RESIZABLE);
        // SetWindowSize(mSpecification.width, mSpecification.height);
        rlw::SetWindowSize(1280, 720);
        rlw::SetTargetFPS(mConfigurationManager.GetTargetFps());
        // SetWindowMinSize(mSpecification.width, mSpecification.height);
        if (mConfigurationManager.GetFullscreen())
        {
            ToggleFullscreen();
        }

#ifndef _DEBUG
        SetExitKey(KEY_NULL);
#endif

        const std::string musicDir = StringFunctionLibrary::NormalizePath(Resources::GetClassicLauncherDir() + "musics");  // theme dir

        mThemes.Init(this);
        mThemes.LoadTheme(this);

        mPrint.LoadFont(Resources::GetFont(), 16, 0);
        mRenderScreen.Init(mSpecification.width, mSpecification.height);

        mAudioManager.Init();
        mAudioManager.LoadMusics(musicDir);
        mAudioManager.LoadCLick(Resources::GetClickAudio());
        mAudioManager.LoadCursor(Resources::GetCursorAudio());

        mSpriteManager.Init();

#ifdef _DEBUG

        // For visual reference you can upload up to four images to guide you
        const std::string refPath0 = StringFunctionLibrary::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/ref0.png");
        const std::string refPath1 = StringFunctionLibrary::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/ref1.png");
        const std::string refPath2 = StringFunctionLibrary::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/ref2.png");
        const std::string refPath3 = StringFunctionLibrary::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/ref3.png");
        mSpriteManager.LoadSprite("ref0", refPath0, 1280 * 2, 720 * 2);
        mSpriteManager.LoadSprite("ref1", refPath1, 1280 * 2, 720 * 2);
        mSpriteManager.LoadSprite("ref2", refPath2, 1280 * 2, 720 * 2);
        mSpriteManager.LoadSprite("ref3", refPath3, 1280 * 2, 720 * 2);
#endif

        rlw::Image imgs[5] = { rlw::LoadImage(Resources::GetIcon(16).c_str()),
                               rlw::LoadImage(Resources::GetIcon(32).c_str()),
                               rlw::LoadImage(Resources::GetIcon(48).c_str()),
                               rlw::LoadImage(Resources::GetIcon(64).c_str()),
                               rlw::LoadImage(Resources::GetIcon(128).c_str()) };

        rlw::SetWindowIcons(imgs, 5);

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
        Loop();

        End();
        rlw::CloseAudioDevice();
        rlw::CloseWindow();

        for (rlw::Image& img : imgs)
        {
            rlw::UnloadImage(img);
        }
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
        while (!rlw::WindowShouldClose())
        {
            if (rlw::IsKeyReleased(rlw::KEY_F11) || (rlw::IsKeyDown(rlw::KEY_LEFT_ALT) && rlw::IsKeyReleased(rlw::KEY_ENTER)))
            {
                ToggleFullscreen();
            }

            // update logic
            Update();  

            // draw in texture render screen
            mRenderScreen.BeginRender();
            mRenderEntities.DrawEntities(mEntityManager.GetEntities()); 
            mRenderScreen.EndRender(); 

            // draw on window
            Draw();        
        }
    }

    void Application::Draw()
    {
        rlw::BeginDrawing();
        rlw::ClearBackground(Color::Black);
        mRenderScreen.Draw();
        mPrint.DrawMessage();
        rlw::EndDrawing();
    }

    void Application::Update()
    {
        // Log(LOG_CLASSIC_DEBUG, TEXTBOOL(InputManager::GetInputLeftFaceLeft()));

        mGuiWindow->Teste();
        mEntityManager.UpdateAll();
        
        mTimerManager.Update();
        mProcessManager.StatusProcessRun(this);
        mInputManager.UpdateInputState();

#ifdef _DEBUG

        GameList* pSystemList = mGameListManager.GetCurrentGameList();
        PRINT(TEXT("========================================"), 2.0f, "line0", Color::Lime);
        PRINT(TEXT("Music Playing %s", mAudioManager.GetMusicName().c_str()), 2.0f, "music", Color::Lime);
        PRINT(TEXT("========================================"), 2.0f, "line", Color::Green);
        PRINT(TEXT("%d fps", rlw::GetFPS()), 2.0f, "fps", Color::Green);
        PRINT(TEXT("%.6f ms", rlw::GetFrameTime()), 2.0f, "ms", Color::Green);
        PRINT(TEXT("========================================"), 2.0f, "line2");
        if (pSystemList)
        {
            PRINT(TEXT("Current game list %s", pSystemList->name.c_str()), 2.0f, "gameList");
        }

        if (rlw::IsKeyReleased(rlw::KEY_F1))
        {
            LogLevel(LOG_CLASSIC_DEBUG, LOG_WARNING);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_DEBUG, LOG_WARNING");
            PRINT("Enabled LOG_CLASSIC_DEBUG, LOG_WARNING", 5.0f);
        }
        if (rlw::IsKeyReleased(rlw::KEY_F2))
        {
            LogLevel(LOG_CLASSIC_DEBUG, LOG_ALL);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_DEBUG, LOG_ALL");
            PRINT("Enabled LOG_CLASSIC_DEBUG, LOG_ALL", 5.0f);
        }
        if (rlw::IsKeyReleased(rlw::KEY_F3))
        {
            LogLevel(LOG_CLASSIC_ALL, LOG_ALL);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_ALL, LOG_ALL");
            PRINT("Enabled LOG_CLASSIC_ALL, LOG_ALL", 5.0f);
        }

        if (InputManager::IsRelease(InputName::rightThumb, main))
        {
            mAudioManager.ChangeMusic();
            mPrint.PrintOnScreen(TEXT("Changed music"), 5.0f);
        }

        if (rlw::IsKeyReleased(rlw::KEY_S))
        {
            mAudioManager.Pause();
            mPrint.PrintOnScreen(TEXT("Pause music"), 5.0f);
        }
        if (rlw::IsKeyReleased(rlw::KEY_D))
        {
            mAudioManager.Play();
            mPrint.PrintOnScreen(TEXT("Play music"), 5.0f);
        }
        if (rlw::IsKeyReleased(rlw::KEY_UP))
        {
            // mEntityManager.SetZOrder(mGuiWindow.get(), 1);
            std::string homeDir = UtilsFunctionLibrary::GetHomeDir();

            LOG(LOG_CLASSIC_DEBUG, TEXT("GetHomeDir %s", homeDir.c_str()));
            LOG(LOG_CLASSIC_DEBUG, TEXT("GetWorkingDirectory %s", UtilsFunctionLibrary::GetWorkingDirectory().c_str()));
            LOG(LOG_CLASSIC_DEBUG, TEXT("GetApplicationDirectory %s", rlw::GetApplicationDirectory()));
        }
#endif

    }


    void Application::End()
    {
        mRenderScreen.Unload();
        mPrint.Unload();
        mAudioManager.Unload();
        mSpriteManager.UnloadSprites();
        mEntityManager.End();
    }

    void Application::ToggleFullscreen()
    {
        bool bIsFullScreen = false;
#ifdef _WIN32
        if (!rlw::IsWindowState(rlw::FLAG_WINDOW_UNDECORATED))
        {
            mSpecification.posWindowX = rlw::GetWindowPosition().x;
            mSpecification.posWindowY = rlw::GetWindowPosition().y;
            mSpecification.width = rlw::GetScreenWidth();
            mSpecification.height = rlw::GetScreenHeight();
            rlw::SetWindowState(rlw::FLAG_WINDOW_UNDECORATED);
            rlw::SetWindowSize(rlw::GetMonitorWidth(rlw::GetCurrentMonitor()), rlw::GetMonitorHeight(rlw::GetCurrentMonitor()));
            const Vector2f positionMonitor(rlw::GetMonitorPosition(rlw::GetCurrentMonitor()));
            rlw::SetWindowPosition((int)positionMonitor.x, (int)positionMonitor.y);
            bIsFullScreen = true;
        }
        else
        {
            rlw::SetWindowSize(mSpecification.width, mSpecification.height);
            rlw::SetWindowPosition(mSpecification.posWindowX, mSpecification.posWindowY);
            rlw::ClearWindowState(rlw::FLAG_WINDOW_UNDECORATED);
            bIsFullScreen = false;
        }
#else
        if (!IsWindowFullscreen())
        {
            mSpecification.posWindowX = rlw::GetWindowPosition().x;
            mSpecification.posWindowY = rlw::GetWindowPosition().y;
            mSpecification.width = rlw::GetScreenWidth();
            mSpecification.height = rlw::GetScreenHeight();
            rlw::SetWindowSize(rlw::GetMonitorWidth(rlw::GetCurrentMonitor()), rlw::GetMonitorHeight(rlw::GetCurrentMonitor()));
            rlw::ToggleFullscreen();
            // rlw::SetConfigFlags(rlw::FLAG_VSYNC_HINT);
            bIsFullScreen = true;
        }
        else
        {
            rlw::ToggleFullscreen();
            rlw::SetWindowSize(mSpecification.width, mSpecification.height);
            rlw::SetWindowPosition(mSpecification.posWindowX, mSpecification.posWindowY);
            bIsFullScreen = false;
        }
#endif
        mConfigurationManager.SetFullscreen(bIsFullScreen);
        mConfigurationManager.SaveConfiguration();
    }
}  // namespace ClassicLauncher