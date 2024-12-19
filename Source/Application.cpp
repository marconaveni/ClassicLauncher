#include "Application.h"
#include "Graphics/Render.h"
#include "GuiComponent.h"
#include "Guis/GuiWindow.h"
#include "Utils/Core.h"
#include "Utils/Resouces.h"
#include "raylib.h"

namespace ClassicLauncher
{

    static Application* sInstanceApplication = nullptr;

    Application::Application() : mEntityManager(&this->mSpriteManager)
    {
        if (sInstanceApplication == nullptr)
        {
            sInstanceApplication = this;
        }
    }

    Application::~Application() { sInstanceApplication = nullptr; }

    Application& Application::Get() { return *sInstanceApplication; }

    ApplicationSpecification Application::GetSpecification() { return mSpecification; }

    Print* Application::GetPrint() { return &mPrint; }

    AudioManager* Application::GetAudioManager() { return &mAudioManager; }

    SpriteManager* Application::GetSpriteManager() { return &mSpriteManager; }

    EntityManager* Application::GetEntityManager() { return &mEntityManager; }

    GameListManager* Application::GetGameListManager() { return &mGameListManager; }

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
        // ToggleFullscreen();

        InitAudioDevice();

        const std::string fontFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::roboto);
        const std::string musicDir = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::musicsFolder);
        const std::string audioClickFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::clickAudio);
        const std::string audioCursorFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::cursorAudio);
        const std::string refPath =
            StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::themesDefaultFolder + "/ref.png");
        const std::string sprite = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::themesSprite);

        mPrint.LoadFont(fontFile, 16, 0);
        mRender.LoadRender(mSpecification.width, mSpecification.height);
        mAudioManager.Init();
        mAudioManager.LoadMusics(musicDir);
        mAudioManager.LoadCLick(audioClickFile);
        mAudioManager.LoadCursor(audioCursorFile);
        mSpriteManager.LoadSprite("ref", refPath);
        mSpriteManager.LoadSprite("sprite", sprite);
        mSpriteManager.LoadSprite("transparent", GenImageColor(1, 1, Color{0, 0, 0, 0}));

        Image img = {
            Resources::iconData, Resources::iconWidth, Resources::iconHeight, 1, Resources::iconFormat,
        };

        SetWindowIcon(img);

        mGuiWindow = mEntityManager.CreateEntity<GuiWindow>();
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
        mPrint.PrintOnScreen(TEXT("name game %s", systemList->name.c_str()), 2.0f, "systemlist", BLUE);

        DrawText(TEXT("%s", mAudioManager.GetMusicName().c_str()), 200, 300, 20, BLACK);
        mPrint.PrintOnScreen(TEXT("Select Game:\n1 - one\n2 - two\n3 - three"), 2.0f, "home", RED);
        mPrint.PrintOnScreen(TEXT("========================================"), 2.0f, "homse", BLUE);
        mPrint.PrintOnScreen(TEXT("Select Game:\n4 - four\n5 - five\n6 - six"), 2.0f, "teste", GREEN);
        mPrint.PrintOnScreen(TEXT("%.6f ", GetFrameTime()), 2.0f, "teste", GREEN);
        // print.PrintOnScreen(TEXT("abc"), 2.0f );

        if (IsKeyReleased(KEY_L))
        {
            mSpriteManager.DeleteSprite("teste");
        }
        if (IsKeyReleased(KEY_K))
        {
            mGameListManager.ChangeSystemToGameList();
        }

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
            std::string homeDir = UtilsFunctionLibrary::GetHomeDir();
            mPrint.Log(LOG_DEBUG, TEXT("GetHomeDir %s", homeDir.c_str()));
            mPrint.Log(LOG_DEBUG, TEXT("GetWorkingDirectory %s", UtilsFunctionLibrary::GetWorkingDirectory().c_str()));
        }
    }

    void Application::Draw()
    {
        mRender.DrawRender();
        mPrint.DrawMessage();
    }

    void Application::EndRender() {}

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
                specification.posWindowX = GetWindowPosition().x;
                specification.posWindowY = GetWindowPosition().y;
                specification.width = GetScreenWidth();
                specification.height = GetScreenHeight();
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                ::ToggleFullscreen();
            }
            else
            {
                ::ToggleFullscreen();
                SetWindowSize(specification.width, specification.height);
                SetWindowPosition(specification.posWindowX, specification.posWindowY);
            }
#endif
        }
    }
}  // namespace ClassicLauncher