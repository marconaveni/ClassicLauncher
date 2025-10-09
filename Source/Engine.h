#ifndef ENGINE_H
#define ENGINE_H


#include <memory>

#include "Application.h"
#include "Audio/AudioManager.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/FontManager.h"
#include "Input/InputManager.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/TimerManager.h"
#include "Window/RayWindow.h"


namespace ClassicLauncher
{

    class Engine
    {
    public:

        Engine();
        ~Engine();
        void Run();

    private:

        ConfigurationManager m_configurationManager;
        RayWindow m_window;
        RenderSystem m_renderSystem;
        InputManager m_inputManager;
        Application m_application;
        SpriteManager m_spriteManager;
        TimerManager m_timerManager;
        AudioManager m_audioManager;
        FontManager m_fontManager;
    };


} // namespace ClassicLauncher


#endif // ENGINE_H