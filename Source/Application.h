#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <vector>
#include "Audio/AudioManager.h"
#include "Core.h"
#include "Data/GameListManager.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "Graphics/Render.h"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteManager.h"
#include "Guis/GuiWindow.h"
#include "Utils/ProcessManager.h"

namespace ClassicLauncher
{
    class GuiWindow;
    class GuiBlackScreen;
    class ProcessManager;

    struct ApplicationSpecification
    {
        int width = 1280;
        int height = 720;
        const char* title = "Classic Launcher";
        int posWindowX = 0;
        int posWindowY = 0;
    };

    class Application
    {
        ApplicationSpecification mSpecification;
        Render mRender;
        Print mPrint;
        AudioManager mAudioManager;
        SpriteManager mSpriteManager;
        EntityManager mEntityManager;
        GameListManager mGameListManager;
        ProcessManager mProcessManager;

        std::shared_ptr<GuiWindow> mGuiWindow;

    public:

        Application();
        ~Application();
        static Application& Get();
        ApplicationSpecification GetSpecification();
        Render* GetRender();
        Print* GetPrint();
        AudioManager* GetAudioManager();
        SpriteManager* GetSpriteManager();
        EntityManager* GetEntityManager();
        GameListManager* GetGameListManager();
        GuiBlackScreen* GetGuiBlackScreen();
        ProcessManager* GetProcessManager();
        void Init();
        void StatusProcessRun();

    private:

        void Loop();
        void Update();
        void Draw();
        void End();
        void ToggleFullscreen();
    };

}  // namespace ClassicLauncher

#endif
