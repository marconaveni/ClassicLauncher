#ifndef RENDER_SCREEN_H
#define RENDER_SCREEN_H

#include <memory>

#include "Data/Rectangle.h"
#include "Data/Vector2.h"

namespace ClassicLauncher
{
    class RenderTexture;

    class RenderScreen
    {
    public:

        RenderScreen();
        ~RenderScreen();
        void Init(int screenWidth, int screenHeight);
        void Clear();
        void BeginRender();
        void EndRender();
        void Draw();
        void Unload();
        [[nodiscard]] Vector2f GetRenderScale() const;
        [[nodiscard]] Vector2f GetMousePositionRender() const;
        [[nodiscard]] int GetScreenWidthGame() const { return static_cast<int>(mWidth); }
        [[nodiscard]] int GetScreenHeightGame() const { return static_cast<int>(mHeight); }

    private:

        void UpdateValues();
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

        std::unique_ptr<RenderTexture> mRenderTexture;
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

} // namespace ClassicLauncher

#endif