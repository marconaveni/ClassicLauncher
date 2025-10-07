#include "Engine.h"

#include <format>
#include <string_view>

#include "ClassicLauncher.h"
#include "Helper.h"
#include "Utils/Resources.h"

namespace ClassicLauncher
{

    Engine::Engine()
        : m_application(m_configurationManager, m_spriteManager, m_timerManager, m_audioManager)
        , m_window(m_configurationManager)
    {
    }

    Engine::~Engine()
    {
    }

    void Engine::Run()
    {

#if _DEBUG
        std::string title = std::format("Classic Launcher [DEVMODE] {}", PRODUCT_VERSION_STRING);
#else
        std::string title = "Classic Launcher";
#endif


        std::vector<std::string> imgs = {Resources::GetIconFile(16).c_str(),
                                         Resources::GetIconFile(32).c_str(),
                                         Resources::GetIconFile(48).c_str(),
                                         Resources::GetIconFile(64).c_str(),
                                         Resources::GetIconFile(128).c_str()};

        m_window.Init(1280, 720, title);
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
        GetPrint()->Unload();
    }


} // namespace ClassicLauncher
