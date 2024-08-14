#include "MainScreen.h"

#include "ImageLoader.h"
#include "ObjectManager.h"
#include "SoundComponent.h"
#include "VideoPlayerComponent.h"
#include "GameListManager.h"
#include "Types.h"

MainScreen::MainScreen() : bIsImageLoaderCallback(false)
{
	grid = std::make_shared<Grid>();
	miniCovers = std::make_shared<MiniCovers>();
	platformProcess = std::make_shared<PlatformProcess>();
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

    // Set the callback to be called when images are loaded
    ImageLoader::GetInstance()->SetCallbackLoadTexture([](Image img, const int indexList, const int indexGame) {
        ImageLoader::GetInstance()->CreateTextures(img, indexList, indexGame);
        });

    ImageLoader::GetInstance()->SetCallbackUnloadTexture([](const std::vector<int>& range, const int indexList) {
        ImageLoader::GetInstance()->UnloadGameListTextureOutRange(range, indexList);
        });
}

void MainScreen::BeginPlay()
{
    //Object::BeginPlay();
}

void MainScreen::Tick()
{
    Object::Tick();


    // Check for callback execution
    {
		bIsImageLoaderCallback = false;

        std::unique_lock<std::mutex> lock(ImageLoader::GetInstance()->queueMutex);
        if (!ImageLoader::GetInstance()->callbackQueue.empty())
        {
			bIsImageLoaderCallback = true;
            auto cb = ImageLoader::GetInstance()->callbackQueue.front();
            ImageLoader::GetInstance()->callbackQueue.pop();
            lock.unlock();
            cb();
        }
    }


    PRINT_STRING(TextFormat("GameList Name: %s", GameListManager::GetInstance()->GetCurrentGameList()->name.c_str()), 0.2f, "nameGame", BLUE);


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
    if (IsKeyReleased(KEY_ENTER) /*&& !bIsImageLoaderCallback*/)
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
            //platformProcess->CreateProc(fullPath, optionalWorkingDirectory);
        }
        else
        {
            GameListManager::GetInstance()->ChangeSystemToGameList();
            grid->SetFocus(3);
            grid->SetCovers();
            miniCovers->SetCovers();
            ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetSystemId(), GameListManager::GetInstance()->GetGameId());
        }
    }
    if (IsKeyReleased(KEY_BACKSPACE))
    {
        if (GameListManager::GetInstance()->GetCurrentList() == GameListSelect)
        {
            GameListManager::GetInstance()->ChangeGameToSystemList();
            grid->SetFocus(3);
            grid->SetCovers();
            miniCovers->SetCovers();
            ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetSystemId(), GameListManager::GetInstance()->GetGameId());
        }
    }
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

