#ifndef ENGINE_H
#define ENGINE_H


#include <memory>
#include "Application.h"
#include "Input/InputManager.h"
#include "Utils/ConfigurationManager.h"
#include "Graphics/RenderSystem.h"
#include "Window/RayWindow.h"


namespace ClassicLauncher
{

    class EntityManager;

    class Engine
    {
    public:

        Engine();
        ~Engine();
        void Run();

    private:

        RayWindow m_window;
        RenderSystem m_renderSystem; 
        InputManager m_inputManager;
        Application m_application;
        ConfigurationManager m_configurationManager;
        
    };


} // namespace ClassicLauncher


#endif // ENGINE_H