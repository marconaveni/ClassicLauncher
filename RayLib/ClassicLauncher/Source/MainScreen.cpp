#include "MainScreen.h"

#include "ImageLoader.h"
#include "ObjectManager.h"
#include "SoundComponent.h"
#include "VideoPlayerComponent.h"
#include "GameListManager.h"
#include "Types.h"

MainScreen::MainScreen()
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
    ImageLoader::GetInstance()->SetCallbackLoadTexture([](Image img, const int i) {
        ImageLoader::GetInstance()->CreateTextures(img, i);
        });

    ImageLoader::GetInstance()->SetCallbackUnloadTexture([](const std::vector<int>& range) {
        ImageLoader::GetInstance()->UnloadGameListTextureOutRange(range);
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
        std::unique_lock<std::mutex> lock(ImageLoader::GetInstance()->queueMutex);
        if (!ImageLoader::GetInstance()->callbackQueue.empty())
        {
            auto cb = ImageLoader::GetInstance()->callbackQueue.front();
            ImageLoader::GetInstance()->callbackQueue.pop();
            lock.unlock();
            cb();
        }
    }


    if (IsKeyReleased(KEY_UP))
    {
        SoundComponent::GetInstance()->PlayClick();
        VideoPlayerComponent::GetInstance()->PlayVideo(GameListManager::GetInstance()->GetCurrentGameList()->video.c_str());
    }
    if (IsKeyReleased(KEY_DOWN))
    {
        SoundComponent::GetInstance()->PlayCursor();
        VideoPlayerComponent::GetInstance()->Stop();
    }
    if (IsKeyReleased(KEY_A))
    {
        SoundComponent::GetInstance()->PauseMusic();
    }
    if (IsKeyReleased(KEY_S))
    {
        std::string fullPath = GameListManager::GetInstance()->GetCurrentSystemList()->executable;
        fullPath.append(" ");
        fullPath.append(GameListManager::GetInstance()->GetCurrentSystemList()->arguments);
        fullPath.append(" \"");
        fullPath.append(GameListManager::GetInstance()->GetCurrentGameList()->path);
        fullPath.append("\" ");
        std::string optionalWorkingDirectory = GetDirectoryPath(GameListManager::GetInstance()->GetCurrentSystemList()->executable.c_str());
        platformProcess->CreateProc(fullPath, optionalWorkingDirectory);
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

