#include "Engine.h"

#include "GameListManager.h"
#include "ObjectManager.h"
#include "VideoPlayerComponent.h"
#include "SoundComponent.h"
#include "ImageLoader.h"
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

	GameListManager::GetInstance()->Initialize();
	GameListManager::GetInstance()->LoadSystemList();
	GameListManager::GetInstance()->LoadGameList();
	SoundComponent::GetInstance()->BeginPlay();
	TextureManager::GetInstance()->LoadResources();
	VideoPlayerComponent::GetInstance()->BeginPlay();
	ImageLoader::GetInstance()->Initialize();

	mainScreen = std::make_shared<MainScreen>();
	mainScreen->Initialize(mainScreen);

	objectManager = std::make_shared<ObjectManager>();
	objectManager->BeginPlay();
}

void Engine::Tick()
{

	objectManager->Tick();
	SoundComponent::GetInstance()->Tick();
	VideoPlayerComponent::GetInstance()->Tick();
	ImageLoader::GetInstance()->Tick();

	objectManager->Draw();
	VideoPlayerComponent::GetInstance()->Draw();

}

void Engine::EndDraw()
{
	objectManager->EndDraw();
	VideoPlayerComponent::GetInstance()->EndDraw();
}

void Engine::EndPlay()
{
	objectManager->EndPlay();
	SoundComponent::GetInstance()->EndPlay();
	VideoPlayerComponent::GetInstance()->EndPlay();
	TextureManager::GetInstance()->EndPlay();
}
