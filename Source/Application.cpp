#include "Application.h"
#include "Graphics/Render.h"
#include "GuiComponent.h"
#include "Guis/GuiWindow.h"
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

    void Application::Init()
    {
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
            EndRender();
        }
    }

    void Application::Update()
    {
        mEntityManager.UpdateAll();
        mEntityManager.UpdatePositionAll();
        mEntityManager.Draw();  // draw in texture render

        // Aqui vai logica
        GameList* systemList = mGameListManager.GetCurrentGameList();
        DrawText(TEXT("%s", mAudioManager.GetMusicName().c_str()), 10, 10, 20, WHITE);

        mPrint.PrintOnScreen(TEXT("\n\n========================================"), 2.0f, "line", GREEN);
        mPrint.PrintOnScreen(TEXT("%d fps", GetFPS()), 2.0f, "fps", GREEN);
        mPrint.PrintOnScreen(TEXT("%.6f ms", GetFrameTime()), 2.0f, "ms", GREEN);
        mPrint.PrintOnScreen(TEXT("========================================"), 2.0f, "line2");
        mPrint.PrintOnScreen(TEXT("Current game list %s", systemList->name.c_str()), 2.0f, "gameList");

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
            mPrint.Log(LOG_DEBUG, TEXT("GetHomeDir %s", homeDir.c_str()));
            mPrint.Log(LOG_DEBUG, TEXT("GetWorkingDirectory %s", UtilsFunctionLibrary::GetWorkingDirectory().c_str()));
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

    void Application::EndRender()
    {
    }

    void Application::End()
    {
        mRender.Unload();
        mPrint.Unload();
        mAudioManager.Unload();
        mSpriteManager.UnloadSprites();
    }

    void Application::ToggleFullscreen()
    {
        if (IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER)))
        {
#ifdef _WIN32
            ToggleBorderlessWindowed();
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