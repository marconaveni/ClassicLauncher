#include "Application.h"

int main()
{
    ClassicLauncher::Application application;
    application.Init();
}

/*
#include <iostream>
//#include "raylib.h"
#include "rl_wrap.h"
using namespace rlw;


#ifndef C_RAYWHITE
#define C_RAYWHITE RAYWHITE
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
                       // Inicializa sistema de áudio
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        std::cout << "AudioDevice não ficou pronto.\n";
        return 1;
    }

    // Carrega uma música (troque por um arquivo .ogg/.mp3/.wav válido no seu projeto)
    Music music = LoadMusicStream("C:/Users/Marco/.ClassicLauncher/musics/package_batocera_emulationstation_es-background-musics_music_Nihilore - Endless Blue.ogg");

    rlw::LogMusicState(music);

    std::cout << "IsMusicValid = " << (rlw::IsMusicValid(music) ? "true" : "false") << "\n";


    // Toca a música por alguns segundos
    PlayMusicStream(music);
    BeginDrawing();

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        UpdateMusicStream(music);

        LogMusicState(music);

        ClearBackground(C_RAYWHITE);

        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

        // Para e descarrega
    StopMusicStream(music);
    UnloadMusicStream(music);

    CloseAudioDevice();
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}  */