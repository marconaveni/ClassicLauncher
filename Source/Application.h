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
    class ThemesManager;
    class TimerManager;
    class FocusManager;
    class ConfigurationManager;
    class RayWindow;

    class Application
    {
    public:

        Application(ConfigurationManager& configManager);
        ~Application();
        
        void Init();
        void Update();
        void Draw();
        void End();

    private:

        RenderEntities mRenderEntities;
        AudioManager mAudioManager;
        SpriteManager mSpriteManager;
        GameListManager mGameListManager;
        ProcessManager mProcessManager;
        ThemesManager mThemes;
        FocusManager mFocusManager;
        InputManager mInputManager;
        TimerManager mTimerManager;
        EntityManager m_entityManager;

        ConfigurationManager* m_configManager;
        GuiWindow* mGuiWindow;
    };

} // namespace ClassicLauncher

#endif
