#include "Application.h"
#include "Graphics/Render.h"
#include "Utils/Resouces.h"
#include "Utils/Core.h"
#include "raylib.h"

namespace ClassicLauncher
{

    static Application* instanceApplication = nullptr;

    Application::Application()
    {   
        if (instanceApplication == nullptr)
        {
            instanceApplication = this;
        }
    }

    Application::~Application()
    {
        instanceApplication = nullptr;    
    }

    Application& Application::Get()
    {
        return *instanceApplication;
    }

    Print* Application::GetPrint()
    {
        return &print;
    }

    void Application::Init()
    {
        SetConfigFlags(FLAG_VSYNC_HINT); // vsync only enable in fullscreen set before InitWindow
        InitWindow(specification.width, specification.height, specification.title);
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        SetWindowMinSize(specification.width, specification.height);
        SetWindowSize(specification.width, specification.height);
        SetTargetFPS(60);
        ChangeDirectory(UtilsFunctionLibrary::GetHomeDir().c_str());
        // SetExitKey(KEY_NULL);
        // ToggleFullscreen();

        InitAudioDevice();
        
        const std::string fontFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::roboto);
        const std::string musicDir = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::musicsFolder);
        const std::string audioClickFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::clickAudio);
        const std::string audioCursorFile = StringFunctionLibrary::NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::cursorAudio);
        print.LoadFont(fontFile, 16, 0);
        render.LoadRender(specification.width, specification.height);
        audioManager.Init();
        audioManager.LoadMusics(musicDir);
        audioManager.LoadCLick(audioClickFile);
        audioManager.LoadCursor(audioCursorFile);





        Loop();

        for(auto& spt : sprites)
        {
            spt->Unload();
        }

        
        End();
        CloseWindow();
        CloseAudioDevice();
    }

    void Application::Loop()
    {
        while (!WindowShouldClose())
        {
            ToggleFullscreen();
            
            render.BeginRender();
            Update();
            render.EndRender();
            
            BeginDrawing();
            ClearBackground(BLACK);

            Draw();

            EndDrawing();
            EndRender();
        }
    }

    void Application::Update()
    {
        // Aqui vai logica 
        int x = 0;
        for(auto& spt : sprites)
        {
            Texture2D* texture = spt->GetSprite();
            if(texture)
            {
                print.PrintOnScreen(TEXT("textura carregada com sucesso?"), 2.0f ,"home33");
                DrawTexture(*texture, x , 200 , WHITE);
                x += 220;
            }
        }
        DrawText(TEXT("%s", audioManager.GetMusicName().c_str()), 200, 300, 20, BLACK);
        print.PrintOnScreen(TEXT("Select Game:\n1 - one\n2 - two\n3 - three"), 2.0f ,"home", RED);
        print.PrintOnScreen(TEXT("========================================"), 2.0f ,"homse", BLUE);
        print.PrintOnScreen(TEXT("Select Game:\n4 - four\n5 - five\n6 - six"), 2.0f, "teste", GREEN);
        //print.PrintOnScreen(TEXT("abc"), 2.0f );

        if(IsKeyReleased(KEY_L))
        {
            sprites.clear();
        }

        if(IsKeyReleased(KEY_A))
        {
            audioManager.ChangeMusic();
            print.PrintOnScreen(TEXT("Changed music"), 2.0f);
        }

        if(IsKeyReleased(KEY_S))
        {
            audioManager.Pause();
            print.PrintOnScreen(TEXT("Pause music"), 2.0f);
        }
        if(IsKeyReleased(KEY_D))
        {
            audioManager.Play();
            print.PrintOnScreen(TEXT("Play music"), 2.0f);
        }
        if(IsKeyReleased(KEY_LEFT))
        {
            audioManager.PlayClick();
            print.PrintOnScreen(TEXT("Click"), 2.0f);
        }
        if(IsKeyReleased(KEY_RIGHT))
        {
            audioManager.PlayCursor();
            print.PrintOnScreen(TEXT("Cursor"), 2.0f);
        }
        if(IsKeyReleased(KEY_UP))
        {
            std::string home_dir = UtilsFunctionLibrary::GetHomeDir();
            print.Log(LOG_DEBUG, TEXT("GetHomeDir %s", home_dir.c_str()));
            print.Log(LOG_DEBUG, TEXT("GetWorkingDirectory %s", UtilsFunctionLibrary::GetWorkingDirectory().c_str()));
        }

    }

    void Application::Draw()
    {
        render.DrawRender();
        print.DrawMessage();
    }

    void Application::EndRender()
    {
    }

    void Application::End()
    {
        render.Unload();
        print.Unload();
        audioManager.Unload();
    }

    void Application::ToggleFullscreen()
    {
        if (IsKeyReleased(KEY_F11))
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
                SetWindowSize(specification.width , specification.height);
                SetWindowPosition(specification.posWindowX, specification.posWindowY);
            }
#endif
        }
    }
}