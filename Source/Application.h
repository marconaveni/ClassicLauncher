#ifndef APPLICATION_H
#define APPLICATION_H

#include "Audio/AudioManager.h"
#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/Render.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/SpriteManager.h"
#include "Guis/GuiWindow.h"
#include "Utils/ProcessManager.h"
#include "Utils/TimerManager.h"
#include "Components/FocusManager.h"

namespace ClassicLauncher
{
    class GuiWindow;
    class GuiBlackScreen;
    class ProcessManager;
    class VideoPlayer;
    class InputManager;
    class Themes;
    class TimerManager;
    class FocusManager;

    struct ApplicationSpecification
    {
        int width = 1280;
        int height = 720;
#if _DEBUG
        const char* title = "Classic Launcher [DEVMODE]";
#else
        const char* title = "Classic Launcher";
#endif
        int posWindowX = 0;
        int posWindowY = 0;
    };

    class Application
    {
        ApplicationSpecification mSpecification;
        Render mRender;
        RenderSystem mRenderSystem;
        Print mPrint;
        AudioManager mAudioManager;
        SpriteManager mSpriteManager;
        EntityManager mEntityManager;
        GameListManager mGameListManager;
        ProcessManager mProcessManager;
        Themes mThemes;
        FocusManager mFocusManager;
        InputManager mInputManager;
        TimerManager mTimerManager;        
        
        GuiWindow* mGuiWindow;

    public:

        Application();
        ~Application();
        static Application& Get();
        ApplicationSpecification GetSpecification() { return mSpecification; }
        Render* GetRender() { return &mRender; }
        RenderSystem* GetRenderSystem() { return &mRenderSystem; }
        Print* GetPrint() { return &mPrint; }
        AudioManager* GetAudioManager() { return &mAudioManager; }
        SpriteManager* GetSpriteManager() { return &mSpriteManager; }
        EntityManager* GetEntityManager() { return &mEntityManager; }
        GameListManager* GetGameListManager() { return &mGameListManager; }
        ProcessManager* GetProcessManager() { return &mProcessManager; }
        Themes* GetThemes() { return &mThemes; }
        TimerManager* GetTimerManager() { return &mTimerManager; }
        FocusManager* GetFocusManager() { return &mFocusManager; }
        GuiBlackScreen* GetGuiBlackScreen();
        void Init();
        void CreateProcess();

    private:

        void Loop();
        void Update();
        void Draw();
        void End();
        void ToggleFullscreen();
    };

}  // namespace ClassicLauncher

#endif
