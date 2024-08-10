


//int main() {
//	std::string str = "AstÃ©rix & ObÃ©lix";
//
//	// Decodificar a string de UTF-8
//	std::string corrected_str = decode_utf8(str);
//
//	std::cout << corrected_str << std::endl;
//
//	return 0;
//}



#include <iostream>
//



#include "raylib.h"
#include "Render.h"
#include "Engine.h"
#include "Print.h"
#include "Types.h"
#include "Date.h"
#include "GameListManager.h"
#include "StringFunctionLibrary.h"
#include "TextureManager.h"

void UpdateDrawFrame();



#include <clocale>
#include "External/tinyxml2.h"



std::vector<std::string> teste()
{
	const char* locale = setlocale(LC_CTYPE, "");
	std::vector<std::string> out;
	tinyxml2::XMLDocument doc;
	//doc.LoadFile("gamelist.xml");

	if(doc.LoadFile("gamelist.xml") != tinyxml2::XMLError::XML_SUCCESS)
	{
		std::cout << "erro ao carregar"  <<"\n";
		return out;
	}

	std::vector<const char*> elems = { "name"/*, "desc", "image", "video", "developer"*/ };

	tinyxml2::XMLElement* pRootElement = doc.RootElement();
	tinyxml2::XMLElement* pGame = pRootElement->FirstChildElement("game");
	std::cout << pRootElement->ChildElementCount("game") << '\n';






	while (pGame) {
		for (std::size_t i{}; i < elems.size(); ++i) {
			tinyxml2::XMLElement* ptr = pGame->FirstChildElement(elems[i]);
			std::string str = ptr->GetText();
			if (str == "3 Ninjas Kick Back")
			{
				ptr->SetText(std::string{ "á é í ã ç" }.c_str());
				//std::cout << "1 Ninja  bom" << "\n";
			}
			//out.emplace_back(ptr->GetText());
		   // std::cout << (i == elems.size() - 1 ? "\n" : "");
		}
		pGame = pGame->NextSiblingElement("game");
	}

	const wchar_t* l = L"\xE1";
	//char l[] = u8"á";
	unsigned char x[] = "é";
	const char* y = "é";


	std::cout << "localization: " << locale << "\n";
	doc.SaveFile("teste.xml");
	return out;
}


int main()
{

	//teste();

	GameListManager::GetInstance()->LoadSystemList();
	GameListManager::GetInstance()->LoadGameList();
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








