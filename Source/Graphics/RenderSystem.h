#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Graphics/RenderScreen.h"

namespace ClassicLauncher
{


    class RenderSystem
    {
    public:

        RenderSystem();
        ~RenderSystem();

        void Init(int width, int height);
        void BeginFrame();
        void EndFrame();
        void BeginDraw();
        void EndDraw();
        void Unload();

    private:

        RenderScreen m_renderScreen{};
    };

} // namespace ClassicLauncher

#endif