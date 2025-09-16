#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "Audio/AudioManager.h"
#include "Components/FocusManager.h"
#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/RenderEntities.h"
#include "Graphics/SpriteManager.h"
#include "Input/InputManager.h"
#include "Themes/Themes.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/ProcessManager.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{

    class RenderScreen;

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

        Application(ConfigurationManager& configManager);
        ~Application();
        static Application& Get();
        AudioManager* GetAudioManager() { return &mAudioManager; }
        // GameListManager* GetGameListManager() { return &mGameListManager; }
        // ProcessManager* GetProcessManager() { return &mProcessManager; }
        Themes* GetThemes() { return &mThemes; }
        FocusManager* GetFocusManager() { return &mFocusManager; }
        // ConfigurationManager* GetConfigurationManager() { return m_configManager; }
        
        void Init();
        void CreateProcess();
        void Update();
        void Draw();
        void End();

    private:

        RenderEntities mRenderEntities;
        AudioManager mAudioManager;
        SpriteManager mSpriteManager;
        GameListManager mGameListManager;
        ProcessManager mProcessManager;
        Themes mThemes;
        FocusManager mFocusManager;
        InputManager mInputManager;
        TimerManager mTimerManager;
        EntityManager m_entityManager;

        ConfigurationManager* m_configManager;
        GuiWindow* mGuiWindow;
    };

} // namespace ClassicLauncher

#endif
