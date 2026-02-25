#include "Engine.h"

#include <format>
#include <string_view>

#include "ClassicLauncher.h"
#include "Helper.h"
#include "Utils/Log.h"
#include "Utils/Resources.h"
#include "Window/WindowSystem.h"

namespace ClassicLauncher
{

    Engine::Engine()
        : m_application(m_configurationManager,
                        m_spriteManager,
                        m_timerManager,
                        m_audioManager,
                        m_fontManager,
                        m_processManager)
        , m_print(m_fontManager)
    {
        RegistryPrint(&m_print);
        WindowSystem::Get().Bind(&m_window);
    }

    Engine::~Engine()
    {
        WindowSystem::Get().Bind(nullptr);
    }

    void Engine::Run()
    {


        std::vector<std::string> imgs = {Resources::GetIconFile(16).c_str(),
                                         Resources::GetIconFile(32).c_str(),
                                         Resources::GetIconFile(48).c_str(),
                                         Resources::GetIconFile(64).c_str(),
                                         Resources::GetIconFile(128).c_str()};

        m_window.Init(WindowSpecs::Width, WindowSpecs::Height, WindowSpecs::Title.data(), m_configurationManager);
        m_window.SetIcons(imgs);
        m_application.Init();
        m_renderSystem.Init(WindowSpecs::Width, WindowSpecs::Height);
        m_print.Init();


        while (!m_window.ShouldClose())
        {
            m_inputManager.UpdateInputState();
            m_application.Update();

            m_renderSystem.BeginFrame(); // desenha dentro da RenderScreen
            m_application.Draw();
            m_renderSystem.EndFrame();

            m_renderSystem.BeginDraw(); // inicia o desenho a render screen
            m_print.DrawMessage();      // note: aqui são desenhos fora da render screen
            m_renderSystem.EndDraw();

            m_window.PoolEvents();

#if _WIN32
            if (m_application.GetStatus() != ProcessStatus::NONE)
            {
                m_application.ProcessUpdate();
            }
#else

            if (m_application.GetStatus() == ProcessStatus::OPEN)
            {
                m_renderSystem.Unload();
                m_spriteManager.Unload();
                m_application.OnGraphicsLost();
                m_window.Close();
                m_processManager.Launch();
                while (m_window.ShouldClose())
                {
                    m_application.ProcessUpdate();
                    if (m_application.GetStatus() == ProcessStatus::CLOSE)
                    {
                        m_window.Init(WindowSpecs::Width,
                                      WindowSpecs::Height,
                                      WindowSpecs::Title.data(),
                                      m_configurationManager);
                        m_window.SetIcons(imgs);
                        m_renderSystem.Init(WindowSpecs::Width, WindowSpecs::Height);
                        m_print.Init();
                        m_application.OnGraphicsRestore();
                    }
                }

#endif
            }
        }

        m_application.End();
    }


} // namespace ClassicLauncher
