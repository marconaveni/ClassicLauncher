#ifndef RENDER_H
#define RENDER_H


#include "Data/Rectangle.h"
#include "Data/Vector2.h"

#include "rl_wrap.h"
using namespace rlw;

namespace ClassicLauncher
{

    class Render
    {
        RenderTexture mRenderTexture;
        RectangleClassic mSource;
        RectangleClassic mDest;
        float mWidth;
        float mHeight;
        float mNewWidth;
        float mNewHeight;
        float mScale;
        bool mIsMaintainAspectRatio;
        Vector2Classic mVirtualMouse;
        void RenderValues();

    public:

        Render();
        void LoadRender(int screenWidth, int screenHeight);
        void ClearRender();
        void BeginRender();
        void EndRender();
        void DrawRender();
        void Unload();
        Vector2Classic GetRenderScale() const;
        Vector2Classic GetMousePositionRender() const;

    private:

        int GetWidthRender() const;
        int GetHeightRender() const;

    public:

        int GetScreenWidthGame() const { return static_cast<int>(mWidth); }
        int GetScreenHeightGame() const { return static_cast<int>(mHeight); }
    };

}  // namespace ClassicLauncher

#endif