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

	ObjectManager::GetInstance()->BeginPlay();
}

void Engine::Tick()
{

	ObjectManager::GetInstance()->Tick();
	SoundComponent::GetInstance()->Tick();
	VideoPlayerComponent::GetInstance()->Tick();
	ImageLoader::GetInstance()->Tick();

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
