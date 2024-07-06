

#include <iostream>
//

#include "raylib.h"
#include "Render.h"
#include "Engine.h"
#include "Print.h"
#include "Types.h"


void UpdateDrawFrame();



int main() 
{
    SetConfigFlags(FLAG_VSYNC_HINT); //vsync only enable in fullscreen set before InitWindow
    InitWindow(screenWidth, screenHeight, TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(screenWidth, screenHeight);
    SetWindowSize(screenWidth, screenHeight ) ;
    SetTargetFPS(60);
    //SetExitKey(KEY_NULL);
    //ToggleFullscreen(); 

    InitAudioDevice();

    Render::GetInstance()->LoadRender(screenWidth, screenHeight);
    Engine::GetInstance()->BeginPlay();

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    Engine::GetInstance()->EndPlay();

    CloseWindow();
    CloseAudioDevice();

	return 0;
}

void UpdateDrawFrame()
{

    if (IsKeyReleased(KEY_F11))
    {
        ToggleBorderlessWindowed();
    }
    Render::GetInstance()->BeginRender();
    Engine::GetInstance()->Tick();
    Render::GetInstance()->EndRender();

    BeginDrawing();
    ClearBackground(BLACK);

    Render::GetInstance()->DrawRender();

    PRINT_STRING(TextFormat("Hello World") , 0.2f, "testmessage", BLUE);
    Print::GetInstance()->DrawMessage();
    EndDrawing();
    Engine::GetInstance()->EndDraw();
}



