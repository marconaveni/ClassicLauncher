#ifndef RENDER_H
#define RENDER_H

#include "Core.h"

namespace ClassicLauncher
{

    class Render
    {
        RenderTexture renderTexture;
        float width;
        float height;
        float newWidth;
        float newHeight;
        float scale;
        bool bMaintainAspectRatio;
        Vector2 virtualMouse;
        void RenderValues();

    public:

        Render();
        void LoadRender(int screenWidth, int screenHeight);
        void BeginRender();
        void EndRender();
        void DrawRender() const;
        void Unload();
        Vector2 GetRenderScale() const;
        Vector2 GetMousePositionRender() const;

    private:

        int GetWidthRender() const;
        int GetHeightRender() const;

    public:

        int GetScreenWidthGame() const { return static_cast<int>(width); }
        int GetScreenHeightGame() const { return static_cast<int>(height); }
    };

}  // namespace ClassicLauncher

#endif