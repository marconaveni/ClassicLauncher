#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include "Audio/AudioManager.h"
#include "Components/FocusManager.h"
#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/RenderEntities.h"
#include "Graphics/RenderScreen.h"
#include "Graphics/SpriteManager.h"
#include "Input/InputManager.h"
#include "Themes/Themes.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/Print.h"
#include "Utils/ProcessManager.h"
#include "Utils/TimerManager.h"

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
    class RayWindow;

    class Application
    {
    public:

        Application();
        ~Application();
        static Application& Get();
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
        std::unique_ptr<RayWindow> m_window;
    };

}  // namespace ClassicLauncher

#endif
