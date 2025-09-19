#include "RenderSystem.h"

#include "Graphics/RenderEntities.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{

    RenderSystem::RenderSystem()
    {
    }

    RenderSystem::~RenderSystem()
    {
    }

    void RenderSystem::Init(int width, int height)
    {
        m_renderScreen.Init(width, height);
    }

    void RenderSystem::BeginFrame()
    {
        m_renderScreen.BeginRender();
    }

    void RenderSystem::EndFrame()
    {
        m_renderScreen.EndRender();
    }

    void RenderSystem::BeginDraw()
    {
        rlw::BeginDrawing(); // esse trecho é o endframe rendersystem
        rlw::ClearBackground(Color::Black);
        m_renderScreen.Draw();
    }

    void RenderSystem::EndDraw()
    {
        rlw::EndDrawing();
    }

} // namespace ClassicLauncher
