#ifndef RENDER_H
#define RENDER_H

#include "Core.h"

namespace ClassicLauncher
{

    class Render
    {
        RenderTexture mRenderTexture;
        Rectangle mSource;
        Rectangle mDest;
        float mWidth;
        float mHeight;
        float mNewWidth;
        float mNewHeight;
        float mScale;
        bool mIsMaintainAspectRatio;
        Vector2 mVirtualMouse;
        void RenderValues();

    public:

        Render();
        void LoadRender(int screenWidth, int screenHeight);
        void ClearRender();
        void BeginRender();
        void EndRender();
        void DrawRender();
        void Unload();
        Vector2 GetRenderScale() const;
        Vector2 GetMousePositionRender() const;

    private:

        int GetWidthRender() const;
        int GetHeightRender() const;

    public:

        int GetScreenWidthGame() const { return static_cast<int>(mWidth); }
        int GetScreenHeightGame() const { return static_cast<int>(mHeight); }
    };

}  // namespace ClassicLauncher

#endif