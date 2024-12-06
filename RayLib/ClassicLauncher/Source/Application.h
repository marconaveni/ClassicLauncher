#ifndef APPLICATION_H
#define APPLICATION_H

#include "Render.h"
#include "Sprite.h"
#include "Core.h"
#include "AudioManager.h"
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
        //TextureManager textureManager;
        AudioManager audioManager;
        std::vector<std::shared_ptr<Sprite>> sprites;


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
