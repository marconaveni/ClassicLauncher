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
	Print::GetInstance()->Log(LOGINFO, "Start Logging");
	TextureManager::GetInstance()->LoadResources();
	mainScreen = std::make_shared<MainScreen>();

	ObjectManager::GetInstance()->RegisterObject(mainScreen);
	ObjectManager::GetInstance()->RegisterObject(mainScreen->grid);
	ObjectManager::GetInstance()->RegisterObject(mainScreen->miniCovers);
	
	mainScreen->grid->RegisterCards();
	mainScreen->miniCovers->RegisterCovers();

	ObjectManager::GetInstance()->BeginPlay();
	VideoPlayerComponent::GetInstance()->BeginPlay();


	SoundComponent::GetInstance()->LoadUiSounds("C:\\Projetos\\ClassicLauncher\\themes\\snes\\snes_sys_click_game.wav", "C:\\Projetos\\ClassicLauncher\\themes\\snes\\snes_sys_cursor.wav");
	SoundComponent::GetInstance()->BeginPlay();
	SoundComponent::GetInstance()->PlayMusic();


	// Set the callback to be called when images are loaded
	ImageLoader::GetInstance()->SetCallbackLoadTexture([](Image img, const int i) {
		ImageLoader::GetInstance()->CreateTextures(img, i);
		});

	ImageLoader::GetInstance()->SetCallbackUnloadTexture([](const std::vector<int>& range) {
		ImageLoader::GetInstance()->UnloadGameListTextureOutRange(range);
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
	}
	if (IsKeyReleased(KEY_A))
	{
		SoundComponent::GetInstance()->PauseMusic();
	}
	if (IsKeyReleased(KEY_S))
	{
		mainScreen->miniCovers->SetCovers();
	}



	ObjectManager::GetInstance()->Tick();
	SoundComponent::GetInstance()->Tick();
	VideoPlayerComponent::GetInstance()->Tick();

	ObjectManager::GetInstance()->Draw();
	VideoPlayerComponent::GetInstance()->Draw();


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
