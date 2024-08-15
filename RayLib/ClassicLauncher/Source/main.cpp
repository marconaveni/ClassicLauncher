

#include <iostream>
#include "raylib.h"
#include "Render.h"
#include "Engine.h"
#include "Print.h"
#include "Types.h"


void UpdateDrawFrame();
void FullscreenToggle();

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT); //vsync only enable in fullscreen set before InitWindow
	InitWindow(screenWidth, screenHeight, TITLE);
	SetWindowState(FLAG_WINDOW_RESIZABLE );
	SetWindowMinSize(screenWidth, screenHeight);
	SetWindowSize(screenWidth, screenHeight);
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

	FullscreenToggle();
	Render::GetInstance()->BeginRender();
	Engine::GetInstance()->Tick();
	Render::GetInstance()->EndRender();

	BeginDrawing();
	ClearBackground(BLACK);

	Render::GetInstance()->DrawRender();
	Print::GetInstance()->DrawMessage();

	EndDrawing();

	Engine::GetInstance()->EndDraw();
}


void FullscreenToggle()
{
	if (IsKeyReleased(KEY_F11))
	{
		ToggleBorderlessWindowed();
#ifndef _WIN32
		if (IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE))
		{
			SetWindowSize(screenWidth, screenHeight);
		}
#endif

	}
}






