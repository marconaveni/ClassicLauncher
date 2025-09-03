#include "Render.h"

#include "Utils/Math.h"
#include "rl_wrap.h"
using namespace rlw;


namespace ClassicLauncher
{

    Render::Render()
        : mRenderTexture(), mWidth(0), mHeight(0), mNewWidth(0), mNewHeight(0), mScale(1), mIsMaintainAspectRatio(true), mVirtualMouse{}
    {
    }

    void Render::RenderValues()
    {
        const Vector2Classic mouse = GetMousePosition();
        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float screenHeight = static_cast<float>(GetScreenHeight());
        mNewWidth = static_cast<float>(GetWidthRender());
        mNewHeight = static_cast<float>(GetHeightRender());

        if (mIsMaintainAspectRatio)
        {
            mScale = Math::Min<float>(screenWidth / mNewWidth, screenHeight / mNewHeight);
            mVirtualMouse.x = (mouse.x - (screenWidth - (mNewWidth * mScale)) * 0.5f) / mScale;
            mVirtualMouse.y = (mouse.y - (screenHeight - (mNewHeight * mScale)) * 0.5f) / mScale;
            mVirtualMouse = Vector2Clamp(mVirtualMouse, Vector2Classic{ 0 }, Vector2Classic{ mNewWidth, mNewHeight });
        }
        else
        {
            mScale = 1;
            mVirtualMouse.x = (mouse.x / screenWidth) * mWidth;
            mVirtualMouse.y = (mouse.y / screenHeight) * mHeight;
        }
    }

    void Render::LoadRender(const int screenWidth, const int screenHeight)
    {
        const float scale = Themes::GetScaleTexture();
        mWidth = screenWidth * scale;
        mHeight = screenHeight * scale;

        mRenderTexture = LoadRenderTexture(mWidth, mHeight);
        SetTextureFilter(mRenderTexture.texture, TEXTURE_FILTER_BILINEAR);
        SetTextureFilter(mRenderTexture.depth, TEXTURE_FILTER_BILINEAR);
    }

    void Render::ClearRender()
    {
        BeginTextureMode(mRenderTexture);
        ClearBackground(C_LIGHTGRAY);
        EndTextureMode();
    }

    void Render::BeginRender()
    {
        RenderValues();
        BeginTextureMode(mRenderTexture);
    }

    void Render::EndRender()
    {
        EndTextureMode();
    }

    void Render::DrawRender()
    {
        Texture* texture = &mRenderTexture.texture;

        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float screenHeight = static_cast<float>(GetScreenHeight());
        const float textureWidth = static_cast<float>(mRenderTexture.texture.width);
        const float textureHeight = static_cast<float>(mRenderTexture.texture.height);

        mSource = RectangleClassic{ 0.0f, 0.0f, textureWidth, -textureHeight };
        mDest = RectangleClassic{ (screenWidth - (mNewWidth * mScale)) * 0.5f, (screenHeight - (mNewHeight * mScale)) * 0.5f, mNewWidth * mScale, mNewHeight * mScale };

        if (IsKeyReleased(KEY_K))
        {
            SetTextureFilter(*texture, TEXTURE_FILTER_POINT);
            SetTextureFilter(mRenderTexture.depth, TEXTURE_FILTER_POINT);

        }
        if (IsKeyReleased(KEY_J))
        {
            SetTextureFilter(*texture, TEXTURE_FILTER_BILINEAR);
            SetTextureFilter(mRenderTexture.depth, TEXTURE_FILTER_BILINEAR);
        }

        // Draw render texture to screen, properly scaled
        DrawTexturePro(*texture, mSource, mDest, Vector2Classic{}, 0.0f, C_WHITE);
    }

    void Render::Unload()
    {
        if (IsRenderTextureValid(mRenderTexture))
        {
            UnloadRenderTexture(mRenderTexture);
            mRenderTexture = RenderTexture{};
        }
    }

    Vector2Classic Render::GetRenderScale() const
    {
        const float scaleWidth = static_cast<float>(GetScreenWidth()) / mWidth;
        const float scaleHeight = static_cast<float>(GetScreenHeight()) / mHeight;
        return Vector2Classic{ scaleWidth, scaleHeight };
    }

    Vector2Classic Render::GetMousePositionRender() const
    {
        return mVirtualMouse;
    }

    int Render::GetWidthRender() const
    {
        return (mIsMaintainAspectRatio) ? static_cast<int>(mWidth) : GetScreenWidth();
    }

    int Render::GetHeightRender() const
    {
        return (mIsMaintainAspectRatio) ? static_cast<int>(mHeight) : GetScreenHeight();
    }

}  // namespace ClassicLauncher