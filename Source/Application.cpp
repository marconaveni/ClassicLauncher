#include "Application.h"
#include "Graphics/Render.h"
#include "Utils/Resouces.h"
#include "Utils/Core.h"
#include "GuiComponent.h"
#include "raylib.h"


namespace ClassicLauncher
{

    static Application* instanceApplication = nullptr;

    Application::Application() :entityManager(&this->spriteManager)
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


	    gameListManager.Initialize();

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


        Image img = {
              Resources::iconData,
              Resources::iconWidth,
              Resources::iconHeight,
              1,
              Resources::iconFormat,
        };

        SetWindowIcon(img);

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
            
            render.BeginRender();
            Update(); // update logic
            entityManager.Draw(); // draw in texture render
            render.EndRender();
            
            BeginDrawing();
            ClearBackground(BLACK);
            Draw(); // draw on screen
            EndDrawing();
            EndRender();
        }
    }

    void Application::Update()
    {
        // Aqui vai logica 

        SystemList* systemList = gameListManager.GetCurrentSystemList();
        print.PrintOnScreen(TEXT("name game %s" , systemList->systemName.c_str() ), 2.0f ,"systemlist", BLUE);

        

        DrawText(TEXT("%s", audioManager.GetMusicName().c_str()), 200, 300, 20, BLACK);
        print.PrintOnScreen(TEXT("Select Game:\n1 - one\n2 - two\n3 - three"), 2.0f ,"home", RED);
        print.PrintOnScreen(TEXT("========================================"), 2.0f ,"homse", BLUE);
        print.PrintOnScreen(TEXT("Select Game:\n4 - four\n5 - five\n6 - six"), 2.0f, "teste", GREEN);
        //print.PrintOnScreen(TEXT("abc"), 2.0f );

        if(IsKeyReleased(KEY_L))
        {
            spriteManager.DeleteSprite("teste");
        }
        if(IsKeyReleased(KEY_K))
        {
        }
        if(IsKeyReleased(KEY_J))
        {
            
            gameListManager.ChangeSystemToGameList();

            int x = 0;
            for (auto& gameList : gameListManager.GetAllGameList())
            {
                const std::string nameId = "cover_" + std::to_string(gameList.mapIndex);
				spriteManager.LoadSprite(gameList.name , gameList.image, 280 , 400 , true);
                const auto guiComponent = entityManager.CreateEntity<GuiComponent>();
                guiComponent->textureName = gameList.name;
                guiComponent->x = x;
                guiComponent->y = 100;
                guiComponent->scale = 0.3f;
                x += 300;
            }
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
            gameListManager.AddId(-1);
            gameListManager.ChangeSystemToGameList();
            audioManager.PlayClick();
            print.PrintOnScreen(TEXT("Click"), 2.0f);
        }
        if(IsKeyReleased(KEY_RIGHT))
        {
            gameListManager.AddId(+1);
            gameListManager.ChangeSystemToGameList();
            audioManager.PlayCursor();
            print.PrintOnScreen(TEXT("Cursor"), 2.0f);
        }
        if(IsKeyReleased(KEY_UP))
        {
            std::string homeDir = UtilsFunctionLibrary::GetHomeDir();
            print.Log(LOG_DEBUG, TEXT("GetHomeDir %s", homeDir.c_str()));
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
        spriteManager.UnloadSprites();
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
                SetWindowSize(specification.width , specification.height);
                SetWindowPosition(specification.posWindowX, specification.posWindowY);
            }
#endif
        }
    }
}