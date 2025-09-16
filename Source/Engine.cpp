#include "Engine.h"

#include <string_view>

#include "Helper.h"
#include "Utils/Resources.h"

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


        std::vector<std::string> imgs = {Resources::GetIcon(16).c_str(), 
                                         Resources::GetIcon(32).c_str(),
                                         Resources::GetIcon(48).c_str(), 
                                         Resources::GetIcon(64).c_str(),
                                         Resources::GetIcon(128).c_str()
        };

        m_window.Init(1280, 720, title.data());
        m_window.SetIcons(imgs);
        m_application.Init();
        m_renderSystem.Init(1280, 720);

        while (!m_window.ShouldClose())
        {
            m_inputManager.UpdateInputState();
            m_application.Update();

            m_renderSystem.BeginFrame(); // desenha dentro da RenderScreen
            m_application.Draw();
            m_renderSystem.EndFrame();

            m_renderSystem.BeginDraw(); // inicia o desenho a render screen
            GetPrint()->DrawMessage();  // note: aqui são desenhos fora da render screen
            m_renderSystem.EndDraw();

            m_window.PoolEvents();
        }

        m_application.End();
    }


} // namespace ClassicLauncher
