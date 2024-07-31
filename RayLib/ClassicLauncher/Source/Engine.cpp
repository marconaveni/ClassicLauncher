#include "Engine.h"
#include <iostream>

#include "GameListManager.h"
#include "Object.h"
#include "ObjectManager.h"
#include "VideoPlayerComponent.h"
#include "SoundComponent.h"
#include "ImageLoader.h"
#include "Types.h"
#include "Print.h"
#include "TextureManager.h"


Engine* Engine::GetInstance()
{
	static Engine object;
	return &object;
}

void Engine::BeginPlay()
{
	TextureManager::GetInstance()->LoadResources();
	mainScreen = std::make_shared<MainScreen>();


	std::shared_ptr<Card> cards;

	ObjectManager::GetInstance()->RegisterObject(mainScreen);
	ObjectManager::GetInstance()->RegisterObject(mainScreen->grid);
	mainScreen->grid->RegisterCards();

	ObjectManager::GetInstance()->BeginPlay();
	VideoPlayerComponent::GetInstance()->BeginPlay();


	//instanceManager.video->PlayVideo("C:\\Projetos\\VisualStudioProjects\\teste\\Blu Cantrell - Hit 'Em Up Style (Oops!) (Video Version).mp4");
   // instanceManager.video->PlayVideo("D:\\Arquivos\\Videos\\Need For Speed Undercover\\Need For Speed(sd).mp4");
   //instanceManager.video->PlayVideo("D:/Arquivos/Videos/Need For Speed 1080P.mp4");


	//SoundComponent::GetInstance()->LoadCurrentMusic("C:\\Projetos\\ClassicLauncher\\musics\\Cyberpunk Synthwave - Nightscapes -- Royalty Free No Copyright Background Music (320 kbps).mp3");
	SoundComponent::GetInstance()->LoadUiSounds("C:\\Projetos\\ClassicLauncher\\themes\\snes\\snes_sys_click_game.wav", "C:\\Projetos\\ClassicLauncher\\themes\\snes\\snes_sys_cursor.wav");
	SoundComponent::GetInstance()->BeginPlay();
	SoundComponent::GetInstance()->PlayMusic();


	// Set the callback to be called when images are loaded
	ImageLoader::GetInstance()->SetCallback([](Image img, int i, bool b) {
		ImageLoader::GetInstance()->CreateTextures(img, i, b);
		});
}

void Engine::Tick()
{

	// Check for callback execution
	{
		std::unique_lock<std::mutex> lock(ImageLoader::GetInstance()->queueMutex);
		if (!ImageLoader::GetInstance()->callbackQueue.empty()) {
			auto cb = ImageLoader::GetInstance()->callbackQueue.front();
			ImageLoader::GetInstance()->callbackQueue.pop();
			lock.unlock();
			cb();
		}
	}


	if (IsKeyReleased(KEY_UP))
	{
		SoundComponent::GetInstance()->PlayCursor();
		VideoPlayerComponent::GetInstance()->PlayVideo("C:\\Projetos\\VisualStudioProjects\\teste\\Blu Cantrell - Hit 'Em Up Style (Oops!) (Video Version).mp4");
	}
	if (IsKeyReleased(KEY_DOWN))
	{
		SoundComponent::GetInstance()->PlayClick();
		VideoPlayerComponent::GetInstance()->PlayVideo("D:\\Arquivos\\Videos\\Need For Speed 1080P.mp4");
		// VideoPlayerComponent::GetInstance()->PlayVideo("C:\\Projetos\\ClassicLauncher\\RayLib\\ClassicLauncher\\Resources\\musics\\Synthwave - Retrowave - Last Stop -- Royalty Free Copyright Safe Music (320 kbps).mp3");
	}
	if (IsKeyReleased(KEY_A))
	{
		SoundComponent::GetInstance()->PauseMusic();
	}
	if (IsKeyReleased(KEY_S))
	{
		SoundComponent::GetInstance()->PlayMusic();
		mainScreen->grid->SetCovers();
	}
	if (IsKeyReleased(KEY_I))
	{
		
	}


	if (IsKeyDown(KEY_LEFT))
	{
		x = x + 10;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		
		x = x - 10;
	}


	ObjectManager::GetInstance()->Tick();
	SoundComponent::GetInstance()->Tick();
	VideoPlayerComponent::GetInstance()->Tick();

	ObjectManager::GetInstance()->Draw();
	VideoPlayerComponent::GetInstance()->Draw();


	for (size_t i = 0; i < ImageLoader::GetInstance()->coversTextures.size(); i++)
	{
		DrawTexture(ImageLoader::GetInstance()->coversTextures[i], (i * ImageLoader::GetInstance()->coversTextures[i].width) + x, 230, WHITE);
	}


}

void Engine::EndDraw()
{
	ObjectManager::GetInstance()->EndDraw();
	VideoPlayerComponent::GetInstance()->EndDraw();
}

void Engine::EndPlay()
{
	ObjectManager::GetInstance()->EndPlay();
	SoundComponent::GetInstance()->EndPlay();
	VideoPlayerComponent::GetInstance()->EndPlay();
	TextureManager::GetInstance()->EndPlay();
}
