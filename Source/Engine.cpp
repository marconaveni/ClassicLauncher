#include "Engine.h"

#include <string_view>
#include "Helper.h"

namespace ClassicLauncher
{
    ConfigurationManager* configInstance = nullptr;

    Engine::Engine()
        : m_application(m_configurationManager), m_window(m_configurationManager), m_renderSystem()
    {
        configInstance = &m_configurationManager;
    }

    Engine::~Engine()
    {
    }

    void Engine::Run()
    {
        m_configurationManager.LoadConfiguration();

#if _DEBUG
        std::string_view title = "Classic Launcher [DEVMODE]";
#else
        std::string_view title = "Classic Launcher";
#endif
        m_window.Init(1280, 720, title.data());
        m_application.Init();
        m_renderSystem.Init(1280, 720);

        while (!m_window.ShouldClose())
        {
            m_inputManager.UpdateInputState();
            m_application.Update();

            m_renderSystem.BeginFrame();
            m_application.Draw();
            m_renderSystem.EndFrame();

            m_renderSystem.BeginDraw();
            GetPrint()->DrawMessage();
            m_renderSystem.EndDraw();

            m_window.PoolEvents();
        }
    }


} // namespace ClassicLauncher
