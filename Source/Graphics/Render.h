#ifndef RENDER_H
#define RENDER_H

#include "Data/Rectangle.h"
#include "Data/Vector2.h"

#include "rl_wrap.h"

namespace ClassicLauncher
{

    class Render
    {
    public:

        Render();
        void LoadRender(int screenWidth, int screenHeight);
        void ClearRender();
        void BeginRender();
        void EndRender();
        void DrawRender();
        void Unload();
        [[nodiscard]] Vector2f GetRenderScale() const;
        [[nodiscard]] Vector2f GetMousePositionRender() const;
        [[nodiscard]] int GetScreenWidthGame() const { return static_cast<int>(mWidth); }
        [[nodiscard]] int GetScreenHeightGame() const { return static_cast<int>(mHeight); }

    private:

        void RenderValues();
        [[nodiscard]] int GetWidthRender() const;
        [[nodiscard]] int GetHeightRender() const;

        rlw::RenderTexture mRenderTexture;
        RectFloat mSource;
        RectFloat mDest;
        float mWidth;
        float mHeight;
        float mNewWidth;
        float mNewHeight;
        float mScale;
        bool mIsMaintainAspectRatio;
        Vector2f mVirtualMouse;
    };

}  // namespace ClassicLauncher

#endif