#ifndef APPLICATION_H
#define APPLICATION_H

#include "Audio/AudioManager.h"
#include "Components/FocusManager.h"
#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/RenderScreen.h"
#include "Graphics/RenderEntities.h"
#include "Graphics/SpriteManager.h"
#include "Input/InputManager.h"
#include "Themes/Themes.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/ProcessManager.h"
#include "Utils/TimerManager.h"
#include "Utils/Print.h"

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
    class ConfigurationManager;

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
    public:

        Application();
        ~Application();
        static Application& Get();
        ApplicationSpecification GetSpecification() { return mSpecification; }
        RenderScreen* GetRenderScreen() { return &mRenderScreen; }
        RenderEntities* GetRenderEntities() { return &mRenderEntities; }
        Print* GetPrint() { return &mPrint; }
        AudioManager* GetAudioManager() { return &mAudioManager; }
        SpriteManager* GetSpriteManager() { return &mSpriteManager; }
        EntityManager* GetEntityManager() { return &mEntityManager; }
        GameListManager* GetGameListManager() { return &mGameListManager; }
        ProcessManager* GetProcessManager() { return &mProcessManager; }
        Themes* GetThemes() { return &mThemes; }
        TimerManager* GetTimerManager() { return &mTimerManager; }
        FocusManager* GetFocusManager() { return &mFocusManager; }
        ConfigurationManager* GetConfigurationManager() { return &mConfigurationManager; }
        GuiBlackScreen* GetGuiBlackScreen();
        void Init();
        void CreateProcess();
        void LoadConfigurationThemes();

    private:

        void Loop();
        void Update();
        void Draw();
        void End();
        void ToggleFullscreen();

        ApplicationSpecification mSpecification;
        RenderScreen mRenderScreen;
        RenderEntities mRenderEntities;
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
        ConfigurationManager mConfigurationManager;

        GuiWindow* mGuiWindow;
    };

}  // namespace ClassicLauncher

#endif
