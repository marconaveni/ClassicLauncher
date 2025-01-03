#include "Render.h"

#include "Utils/Math.h"
#include "raylib.h"
#include "raymath.h"  // Required for: Vector2Clamp()

namespace ClassicLauncher
{

    Render::Render()
        : mRenderTexture(), mWidth(0), mHeight(0), mNewWidth(0), mNewHeight(0), mScale(1), mIsMaintainAspectRatio(true), mVirtualMouse{}
    {
    }

    void Render::RenderValues()
    {
        const Vector2 mouse = GetMousePosition();
        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float screenHeight = static_cast<float>(GetScreenHeight());
        mNewWidth = static_cast<float>(GetWidthRender());
        mNewHeight = static_cast<float>(GetHeightRender());

        if (mIsMaintainAspectRatio)
        {
            mScale = Math::Min<float>(screenWidth / mNewWidth, screenHeight / mNewHeight);
            mVirtualMouse.x = (mouse.x - (screenWidth - (mNewWidth * mScale)) * 0.5f) / mScale;
            mVirtualMouse.y = (mouse.y - (screenHeight - (mNewHeight * mScale)) * 0.5f) / mScale;
            mVirtualMouse = Vector2Clamp(mVirtualMouse, Vector2{ 0 }, Vector2{ mNewWidth, mNewHeight });
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
        mWidth = screenWidth;
        mHeight = screenHeight;
        mRenderTexture = LoadRenderTexture(screenWidth, screenHeight);
        GenTextureMipmaps(&mRenderTexture.texture);
        SetTextureFilter(mRenderTexture.texture, TEXTURE_FILTER_TRILINEAR);
        GenTextureMipmaps(&mRenderTexture.depth);
        SetTextureFilter(mRenderTexture.depth, TEXTURE_FILTER_TRILINEAR);
    }

    void Render::BeginRender()
    {
        RenderValues();
        BeginTextureMode(mRenderTexture);
        ClearBackground(LIGHTGRAY);
    }

    void Render::EndRender()
    {
        EndTextureMode();
    }

    void Render::DrawRender() const
    {
        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float screenHeight = static_cast<float>(GetScreenHeight());
        const float textureWidth = static_cast<float>(mRenderTexture.texture.width);
        const float textureHeight = static_cast<float>(mRenderTexture.texture.height);

        const Texture2D texture = mRenderTexture.texture;
        const Rectangle source = Rectangle{ 0.0f, 0.0f, textureWidth, -textureHeight };
        const Rectangle dest = Rectangle{ (screenWidth - (mNewWidth * mScale)) * 0.5f, (screenHeight - (mNewHeight * mScale)) * 0.5f, mNewWidth * mScale, mNewHeight * mScale };

        // Draw render texture to screen, properly scaled
        DrawTexturePro(texture, source, dest, Vector2{}, 0.0f, WHITE);
    }

    void Render::Unload()
    {
        if (IsRenderTextureValid(mRenderTexture))
        {
            UnloadRenderTexture(mRenderTexture);
            mRenderTexture = RenderTexture{};
        }
    }

    Vector2 Render::GetRenderScale() const
    {
        const float scaleWidth = static_cast<float>(GetScreenWidth()) / mWidth;
        const float scaleHeight = static_cast<float>(GetScreenHeight()) / mHeight;
        return Vector2{ scaleWidth, scaleHeight };
    }

    Vector2 Render::GetMousePositionRender() const
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