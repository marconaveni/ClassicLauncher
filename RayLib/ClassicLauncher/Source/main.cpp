

#include <iostream>
#include "raylib.h"
#include "Render.h"
#include "Engine.h"
#include "Print.h"
#include "Types.h"
#include "Date.h"
#include "GameListManager.h"
#include "TextureManager.h"
#include "External/tinyxml2.h"


void UpdateDrawFrame();


int main()
{


	SetConfigFlags(FLAG_VSYNC_HINT); //vsync only enable in fullscreen set before InitWindow
	InitWindow(screenWidth, screenHeight, TITLE);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
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

	//DrawTexture(LoadTexture("Resources/themes/default/sprite.png"), 0, 0, RED);
	PRINT_STRING(TextFormat("Hello World"), 0.2f, "testmessage", BLUE);
	DateTime dt;
	DateTime dty;

	dt.GetCurrentTimeAndDate();
	dty.GetCurrentTimeAndDate();
	PRINT_STRING(TextFormat("%s", dty.FormatDateTimeToXml().c_str()), 0.2f, "dateformat", BLUE);



	PRINT_STRING(TextFormat("%d:%d:%d  %d-%d-%d", dty.hour, dty.minute, dty.second, dty.day, dty.month, dty.year), 0.2f, "datebefore", BLUE);
	//dt = "20231228T230138" ;
	dt = "20231228T230138";
	dt = "20231228T230138";
	dty = "20231228T230138";
	PRINT_STRING(TextFormat("%d:%d:%d  %d-%d-%d", dt.hour, dt.minute, dt.second, dt.day, dt.month, dt.year), 0.2f, "date", BLUE);

	if (dty <= dt)
	{
		PRINT_STRING(TextFormat("é verdadeiro"), 0.2f, "tffff", BLUE);
	}
	else
	{
		PRINT_STRING(TextFormat("falso"), 0.2f, "tfff", BLUE);
	}

	Print::GetInstance()->DrawMessage();
	EndDrawing();
	Engine::GetInstance()->EndDraw();
}








