#include "Application.h"
#include "Graphics/Render.h"
#include "Guis/GuiComponent.h"
#include "Guis/GuiWindow.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/Log.h"
#include "Utils/Resources.h"

namespace ClassicLauncher
{

    static Application* sInstanceApplication = nullptr;

    Application::Application()
        : mRenderSystem(&this->mSpriteManager), mEntityManager(&this->mSpriteManager, &this->mTimerManager), mGuiWindow(nullptr)
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
        SetTraceLogCallback(TraceLogger);

        Resources::SetClassicLauncherDir();
        mGameListManager.Initialize();

        InitAudioDevice();

        if (mConfigurationManager.GetVSync())
        {
            SetConfigFlags(FLAG_VSYNC_HINT);  // vsync only enable in fullscreen set before InitWindow
        }
        InitWindow(mSpecification.width, mSpecification.height, mSpecification.title);
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        SetWindowSize(mSpecification.width, mSpecification.height);
        SetTargetFPS(mConfigurationManager.GetTargetFps());
        SetWindowMinSize(mSpecification.width, mSpecification.height);
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
        mRender.LoadRender(mSpecification.width, mSpecification.height);

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
        mSpriteManager.LoadSprite("ref0", refPath0);
        mSpriteManager.LoadSprite("ref1", refPath1);
        mSpriteManager.LoadSprite("ref2", refPath2);
        mSpriteManager.LoadSprite("ref3", refPath3);
#endif

        Image imgs[5] = { LoadImage(Resources::GetIcon(16).c_str()),
                          LoadImage(Resources::GetIcon(32).c_str()),
                          LoadImage(Resources::GetIcon(48).c_str()),
                          LoadImage(Resources::GetIcon(64).c_str()),
                          LoadImage(Resources::GetIcon(128).c_str()) };

        SetWindowIcons(imgs, 5);

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
        CloseAudioDevice();
        CloseWindow();

        for (Image& img : imgs)
        {
            UnloadImage(img);
        }
    }

    void Application::CreateProcess()
    {
        GetAudioManager()->Pause();
        GetProcessManager()->CreateProc(this);
    }

    void Application::Loop()
    {
        while (!WindowShouldClose())
        {
            if (IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER)))
            {
                ToggleFullscreen();
            }

            BeginDrawing();
            ClearBackground(BLACK);

            mRender.ClearRender();
            mRender.BeginRender();
            Update();  // update logic
            mRender.EndRender();

            Draw();  // draw on screen
            EndDrawing();
        }
    }

    void Application::Update()
    {
        mInputManager.UpdateInputState();
        mEntityManager.UpdateAll();
        mRenderSystem.DrawEntities(mEntityManager.GetEntities());  // draw in texture render  // mEntityManager.Draw();  // draw in texture render
        mTimerManager.Update();
        mGuiWindow->Teste();

        // Log(LOG_CLASSIC_DEBUG, TEXTBOOL(InputManager::GetInputLeftFaceLeft()));

        mProcessManager.StatusProcessRun(this);

        GameList* pSystemList = mGameListManager.GetCurrentGameList();
        PRINT(TEXT("========================================"), 2.0f, "line0", Color::Lime());
        PRINT(TEXT("Music Playing %s", mAudioManager.GetMusicName().c_str()), 2.0f, "music", Color::Lime());
        PRINT(TEXT("========================================"), 2.0f, "line", Color::Green());
        PRINT(TEXT("%d fps", GetFPS()), 2.0f, "fps", Color::Green());
        PRINT(TEXT("%.6f ms", GetFrameTime()), 2.0f, "ms", Color::Green());
        PRINT(TEXT("========================================"), 2.0f, "line2");
        if (pSystemList)
        {
            PRINT(TEXT("Current game list %s", pSystemList->name.c_str()), 2.0f, "gameList");
        }

#ifdef _DEBUG

        if (IsKeyReleased(KEY_F1))
        {
            LogLevel(LOG_CLASSIC_DEBUG, LOG_WARNING);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_DEBUG, LOG_WARNING");
            PRINT("Enabled LOG_CLASSIC_DEBUG, LOG_WARNING", 5.0f);
        }
        if (IsKeyReleased(KEY_F2))
        {
            LogLevel(LOG_CLASSIC_DEBUG, LOG_ALL);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_DEBUG, LOG_ALL");
            PRINT("Enabled LOG_CLASSIC_DEBUG, LOG_ALL", 5.0f);
        }
        if (IsKeyReleased(KEY_F3))
        {
            LogLevel(LOG_CLASSIC_ALL, LOG_ALL);
            LOG(LOG_CLASSIC_DEBUG, "Enabled LOG_CLASSIC_ALL, LOG_ALL");
            PRINT("Enabled LOG_CLASSIC_ALL, LOG_ALL", 5.0f);
        }

#endif

        if (InputManager::IsRelease(InputName::rightThumb, main))
        {
            mAudioManager.ChangeMusic();
            mPrint.PrintOnScreen(TEXT("Changed music"), 5.0f);
        }

        if (IsKeyReleased(KEY_S))
        {
            mAudioManager.Pause();
            mPrint.PrintOnScreen(TEXT("Pause music"), 5.0f);
        }
        if (IsKeyReleased(KEY_D))
        {
            mAudioManager.Play();
            mPrint.PrintOnScreen(TEXT("Play music"), 5.0f);
        }
        if (IsKeyReleased(KEY_UP))
        {
            // mEntityManager.SetZOrder(mGuiWindow.get(), 1);
            std::string homeDir = UtilsFunctionLibrary::GetHomeDir();

            LOG(LOG_CLASSIC_DEBUG, TEXT("GetHomeDir %s", homeDir.c_str()));
            LOG(LOG_CLASSIC_DEBUG, TEXT("GetWorkingDirectory %s", UtilsFunctionLibrary::GetWorkingDirectory().c_str()));
            LOG(LOG_CLASSIC_DEBUG, TEXT("GetApplicationDirectory %s", ::GetApplicationDirectory()));
        }
    }

    void Application::Draw()
    {
        mRender.DrawRender();
        mPrint.DrawMessage();
    }

    void Application::End()
    {
        mRender.Unload();
        mPrint.Unload();
        mAudioManager.Unload();
        mSpriteManager.UnloadSprites();
        mEntityManager.End();
    }

    void Application::ToggleFullscreen()
    {
        bool bIsFullScreen = false;
#ifdef _WIN32
        if (!IsWindowState(FLAG_WINDOW_UNDECORATED))
        {
            mSpecification.posWindowX = GetWindowPosition().x;
            mSpecification.posWindowY = GetWindowPosition().y;
            mSpecification.width = GetScreenWidth();
            mSpecification.height = GetScreenHeight();
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            const Vector2 positionMonitor(GetMonitorPosition(GetCurrentMonitor()));
            SetWindowPosition(positionMonitor.GetIntX(), positionMonitor.GetIntY());
            bIsFullScreen = true;
        }
        else
        {
            SetWindowSize(mSpecification.width, mSpecification.height);
            SetWindowPosition(mSpecification.posWindowX, mSpecification.posWindowY);
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            bIsFullScreen = false;
        }
#else
        if (!IsWindowFullscreen())
        {
            mSpecification.posWindowX = GetWindowPosition().x;
            mSpecification.posWindowY = GetWindowPosition().y;
            mSpecification.width = GetScreenWidth();
            mSpecification.height = GetScreenHeight();
            SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            ::ToggleFullscreen();
            SetConfigFlags(FLAG_VSYNC_HINT);
            bIsFullScreen = true;
        }
        else
        {
            ::ToggleFullscreen();
            SetWindowSize(mSpecification.width, mSpecification.height);
            SetWindowPosition(mSpecification.posWindowX, mSpecification.posWindowY);
            bIsFullScreen = false;
        }
#endif
        mConfigurationManager.SetFullscreen(bIsFullScreen);
        mConfigurationManager.SaveConfiguration();
    }
}  // namespace ClassicLauncher