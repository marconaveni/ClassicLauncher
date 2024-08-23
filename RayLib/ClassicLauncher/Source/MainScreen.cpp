#include "MainScreen.h"

#include "ImageLoader.h"
#include "ObjectManager.h"
#include "SoundComponent.h"
#include "VideoPlayerComponent.h"
#include "GameListManager.h"
#include "TextureManager.h"
#include "Types.h"

MainScreen::MainScreen()
{
	grid = std::make_shared<Grid>();
	miniCovers = std::make_shared<MiniCovers>();
	platformProcess = std::make_shared<PlatformProcess>();

	// Set the callback to be called when images are loaded
	threadLoad.SetCallback([this]() {
		ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetGameId());
		});
}


void MainScreen::Initialize(const std::shared_ptr<MainScreen>& mainScreenRef)
{
	ObjectManager::GetInstance()->RegisterObject(mainScreenRef);
	ObjectManager::GetInstance()->RegisterObject(grid);
	ObjectManager::GetInstance()->RegisterObject(miniCovers);
	ObjectManager::GetInstance()->RegisterObject(platformProcess);

	grid->RegisterCards();
	miniCovers->RegisterCovers();

	SoundComponent::GetInstance()->LoadUiSounds(PATH_SOUND_CLICK, PATH_SOUND_CURSOR);
	SoundComponent::GetInstance()->PlayMusic();


}

void MainScreen::BeginPlay()
{
	//Object::BeginPlay();
}

void MainScreen::Tick()
{
	Object::Tick();


	if (GameListManager::GetInstance()->GetCurrentGameList() == nullptr)
	{
		LOG(LOGERROR, TextFormat("GameList is Empty \nLine %d in file %s", __LINE__, __FILE__));
		return;
	}


	PRINT_STRING(TextFormat("GameList Name: %s", GameListManager::GetInstance()->GetCurrentGameList()->name.c_str()), 0.2f, "Game", BLUE);
	PRINT_STRING(TextFormat("GameList mapIndex: %d", GameListManager::GetInstance()->GetCurrentGameList()->mapIndex), 0.2f, "Game1", BLUE);


	if (IsKeyReleased(KEY_UP))
	{
		SoundComponent::GetInstance()->PlayCursor();
		VideoPlayerComponent::GetInstance()->Stop();
	}
	if (IsKeyReleased(KEY_DOWN))
	{
		SoundComponent::GetInstance()->PlayClick();
		VideoPlayerComponent::GetInstance()->PlayVideo(GameListManager::GetInstance()->GetCurrentGameList()->video.c_str());
	}
	if (IsKeyReleased(KEY_A))
	{
		SoundComponent::GetInstance()->PauseMusic();
	}
	if (IsKeyReleased(KEY_ENTER))
	{
		SoundComponent::GetInstance()->PlayClick();

		if (GameListManager::GetInstance()->GetCurrentList() == GameListSelect)
		{
			std::string fullPath = GameListManager::GetInstance()->GetCurrentSystemList()->executable;
			fullPath.append(" ");
			fullPath.append(GameListManager::GetInstance()->GetCurrentSystemList()->arguments);
			fullPath.append(" \"");
			fullPath.append(GameListManager::GetInstance()->GetCurrentGameList()->path);
			fullPath.append("\" ");
			const std::string optionalWorkingDirectory = GetDirectoryPath(GameListManager::GetInstance()->GetCurrentSystemList()->executable.c_str());
			platformProcess->CreateProc(fullPath, optionalWorkingDirectory);
		}
		else
		{
			ClearCovers();
			threadLoad.StartThread(&MainScreen::ChangeGrid, this, GameListSelect);
		}
	}
	if (IsKeyReleased(KEY_BACKSPACE))
	{
		if (GameListManager::GetInstance()->GetCurrentList() == GameListSelect)
		{
			ClearCovers();
			threadLoad.StartThread(&MainScreen::ChangeGrid, this, SystemListSelect);
		}
	}

	threadLoad.CallbackExecution();
}

void MainScreen::ChangeGrid(const CurrentList list)
{
	
	grid->SetFocus(3);
	if (list == SystemListSelect)
	{
		GameListManager::GetInstance()->ChangeGameToSystemList();
	}
	else
	{
		GameListManager::GetInstance()->ChangeSystemToGameList();
	}

	threadLoad.Notify();
	//ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetGameId());
}

void MainScreen::Draw()
{
	//Object::Draw();
}

void MainScreen::EndDraw()
{
	//Object::EndDraw();
}

void MainScreen::EndPlay()
{
	//Object::EndPlay();
}

void MainScreen::Collision()
{
	//Object::Collision();
}

void MainScreen::ClearCovers()
{
	TextureManager::GetInstance()->ClearCovers();
	grid->ClearCovers();
	miniCovers->ClearCovers();
}

