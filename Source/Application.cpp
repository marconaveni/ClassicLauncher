#include "Application.h"
#include "Graphics/Render.h"
#include "GuiComponent.h"
#include "Guis/GuiWindow.h"
#include "Utils/Log.h"
#include "Utils/Resources.h"

namespace ClassicLauncher
{

    static Application* sInstanceApplication = nullptr;

    Application::Application()
        : mEntityManager(&this->mSpriteManager)
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

    ApplicationSpecification Application::GetSpecification()
    {
        return mSpecification;
    }

    Render* Application::GetRender()
    {
        return &mRender;
    }

    Print* Application::GetPrint()
    {
        return &mPrint;
    }

    AudioManager* Application::GetAudioManager()
    {
        return &mAudioManager;
    }

    SpriteManager* Application::GetSpriteManager()
    {
        return &mSpriteManager;
    }

    EntityManager* Application::GetEntityManager()
    {
        return &mEntityManager;
    }

    GameListManager* Application::GetGameListManager()
    {
        return &mGameListManager;
    }

    GuiBlackScreen* Application::GetGuiBlackScreen()
    {
        return mGuiWindow->GetGuiBlackScreen();
    }

    ProcessManager* Application::GetProcessManager()
    {
        return &mProcessManager;
    }

    void Application::Init()
    {
        LogLevel(LOG_CLASSIC_DEBUG, LOG_WARNING);
        SetTraceLogCallback(TraceLogger);

        mGameListManager.Initialize();

        SetConfigFlags(FLAG_VSYNC_HINT);  // vsync only enable in fullscreen set before InitWindow
        InitWindow(mSpecification.width, mSpecification.height, mSpecification.title);
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        SetWindowMinSize(mSpecification.width, mSpecification.height);
        SetWindowSize(mSpecification.width, mSpecification.height);
        SetTargetFPS(60);
        ChangeDirectory(UtilsFunctionLibrary::GetHomeDir().c_str());
        // SetExitKey(KEY_NULL);

        InitAudioDevice();

        const std::string fontFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::roboto);
        const std::string musicDir = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::musicsFolder);
        const std::string audioClickFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::clickAudio);
        const std::string audioCursorFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::cursorAudio);
        const std::string refPath = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::themesDefaultFolder + "/ref.png");
        const std::string sprite = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::themesSprite);

        mPrint.LoadFont(fontFile, 16, 0);
        mRender.LoadRender(mSpecification.width, mSpecification.height);
        mAudioManager.Init();
        mAudioManager.LoadMusics(musicDir);
        mAudioManager.LoadCLick(audioClickFile);
        mAudioManager.LoadCursor(audioCursorFile);
        mSpriteManager.Init();
        mSpriteManager.LoadSprite("ref", refPath);
        mSpriteManager.LoadSprite("sprite", sprite);

        Image img = {
            Resources::iconData, Resources::iconWidth, Resources::iconHeight, 1, Resources::iconFormat,
        };

        SetWindowIcon(img);

        mGuiWindow = mEntityManager.CreateEntity<GuiWindow>("GuiWindow");
        mGuiWindow->Init();

        Loop();

        End();
        CloseWindow();
        CloseAudioDevice();
    }

    void Application::StatusProcessRun()
    {
        ProcessStatus status = mProcessManager.UpdateRun();

        switch (status)
        {
            case ProcessStatus::None:
                break;
            case ProcessStatus::Open:
                mAudioManager.Pause();
                break;
            case ProcessStatus::Running:
                WaitTime(2.5);
                break;
            case ProcessStatus::Close:
                GetGuiBlackScreen()->KeepBlack();
                mAudioManager.ChangeMusic();
                InputManager::EnableInput();
                break;
            default:
                break;
        }
    }

    void Application::Loop()
    {
        while (!WindowShouldClose())
        {
            ToggleFullscreen();

            mRender.BeginRender();
            Update();  // update logic
            mRender.EndRender();

            BeginDrawing();
            ClearBackground(BLACK);
            Draw();  // draw on screen
            EndDrawing();
        }
    }

    void Application::Update()
    {
        mEntityManager.UpdateAll();
        mEntityManager.UpdatePositionAll();
        mEntityManager.Draw();  // draw in texture render

        // Log(LOG_CLASSIC_DEBUG, TEXTBOOL(InputManager::GetInputLeftFaceLeft()));

        StatusProcessRun();

        GameList* systemList = mGameListManager.GetCurrentGameList();
        PRINT(TEXT("========================================"), 2.0f, "line0", Color::Lime());
        PRINT(TEXT("Music Playing %s", mAudioManager.GetMusicName().c_str()), 2.0f, "music", Color::Lime());
        PRINT(TEXT("========================================"), 2.0f, "line", Color::Green());
        PRINT(TEXT("%d fps", GetFPS()), 2.0f, "fps", Color::Green());
        PRINT(TEXT("%.6f ms", GetFrameTime()), 2.0f, "ms", Color::Green());
        PRINT(TEXT("========================================"), 2.0f, "line2");
        PRINT(TEXT("Current game list %s", systemList->name.c_str()), 2.0f, "gameList");

#ifdef _DEBUG

        if (IsKeyReleased(KEY_ONE))
        {
            LogLevel(LOG_CLASSIC_DEBUG, LOG_WARNING);
            Log(LOG_CLASSIC_DEBUG, "Enable LOG_CLASSIC_DEBUG, LOG_WARNING");
        }
        if (IsKeyReleased(KEY_TWO))
        {
            LogLevel(LOG_CLASSIC_DEBUG, LOG_ALL);
            Log(LOG_CLASSIC_DEBUG, "Enable LOG_CLASSIC_DEBUG, LOG_ALL");
        }
        if (IsKeyReleased(KEY_THREE))
        {
            LogLevel(LOG_CLASSIC_ALL, LOG_ALL);
            Log(LOG_CLASSIC_DEBUG, "Enable LOG_CLASSIC_ALL, LOG_ALL");
        }

#endif

        if (IsKeyReleased(KEY_A))
        {
            mAudioManager.ChangeMusic();
            mPrint.PrintOnScreen(TEXT("Changed music"), 2.0f);
        }

        if (IsKeyReleased(KEY_S))
        {
            mAudioManager.Pause();
            mPrint.PrintOnScreen(TEXT("Pause music"), 2.0f);
        }
        if (IsKeyReleased(KEY_D))
        {
            mAudioManager.Play();
            mPrint.PrintOnScreen(TEXT("Play music"), 2.0f);
        }
        if (IsKeyReleased(KEY_UP))
        {
            // mEntityManager.SetZOrder(mGuiWindow.get(), 1);
            std::string homeDir = UtilsFunctionLibrary::GetHomeDir();

            Log(LOG_CLASSIC_DEBUG, TEXT("GetHomeDir %s", homeDir.c_str()));
            Log(LOG_CLASSIC_DEBUG, TEXT("GetWorkingDirectory %s", UtilsFunctionLibrary::GetWorkingDirectory().c_str()));
        }
        if (IsKeyDown(KEY_DOWN))
        {
            mGuiWindow->SetBringToFront();
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
        if (IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER)))
        {
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
            }
            else
            {
                SetWindowSize(mSpecification.width, mSpecification.height);
                SetWindowPosition(mSpecification.posWindowX, mSpecification.posWindowY);
                ClearWindowState(FLAG_WINDOW_UNDECORATED);
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
            }
            else
            {
                ::ToggleFullscreen();
                SetWindowSize(mSpecification.width, mSpecification.height);
                SetWindowPosition(mSpecification.posWindowX, mSpecification.posWindowY);
            }
#endif
        }
    }
}  // namespace ClassicLauncher