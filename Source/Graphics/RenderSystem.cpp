#include "RenderSystem.h"

#include "Graphics/RenderEntities.h"
#include "Window/Window.h"
#include "Wrap.h"

namespace ClassicLauncher
{

    RenderSystem::RenderSystem(Window* window)
        : m_renderScreen(window)
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
        rlw::BeginDrawing();
        rlw::ClearBackground(Color::Black);
    }

    void RenderSystem::DrawRender()
    {
        m_renderScreen.Draw();
    }

    void RenderSystem::EndDraw()
    {
        rlw::EndDrawing();
    }

    void RenderSystem::Unload()
    {
        m_renderScreen.Unload();
    }

} // namespace ClassicLauncher
