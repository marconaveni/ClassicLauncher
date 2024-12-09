#ifndef APPLICATION_H
#define APPLICATION_H

#include "Graphics/Render.h"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteManager.h"
#include "Utils/Core.h"
#include "Audio/AudioManager.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include <vector>
#include <memory>

namespace ClassicLauncher
{


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
        ApplicationSpecification specification;
        Render render;
        Print print;
        AudioManager audioManager;
        EntityManager entityManager;
        SpriteManager spriteManager;

    public:
        Application();
        ~Application();
        static Application& Get();
        Print* GetPrint();
        void Init();

    private:
        void Loop();
        void Update();
        void Draw();
        void EndRender();
        void End();
        void ToggleFullscreen();

    };
    
} // namespace ClassicLauncher


#endif 
