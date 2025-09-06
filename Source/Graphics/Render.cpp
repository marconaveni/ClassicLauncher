#include "Render.h"

#include "Utils/Math.h"
#include "Themes/Themes.h"
#include "rl_wrap.h"



namespace ClassicLauncher
{

    Render::Render()
        : mRenderTexture(), mWidth(0), mHeight(0), mNewWidth(0), mNewHeight(0), mScale(1), mIsMaintainAspectRatio(true), mVirtualMouse{}
    {
    }

    void Render::RenderValues()
    {
        const Vector2f mouse = rlw::GetMousePosition();
        const auto screenWidth = static_cast<float>(rlw::GetScreenWidth());
        const auto screenHeight = static_cast<float>(rlw::GetScreenHeight());
        mNewWidth = static_cast<float>(GetWidthRender());
        mNewHeight = static_cast<float>(GetHeightRender());

        if (mIsMaintainAspectRatio)
        {
            mScale = Math::Min<float>(screenWidth / mNewWidth, screenHeight / mNewHeight);
            mVirtualMouse.x = (mouse.x - (screenWidth - (mNewWidth * mScale)) * 0.5f) / mScale;
            mVirtualMouse.y = (mouse.y - (screenHeight - (mNewHeight * mScale)) * 0.5f) / mScale;
            mVirtualMouse = rlw::Vector2Clamp(mVirtualMouse, { 0.0f ,0.0f }, { mNewWidth, mNewHeight });
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

        mRenderTexture = rlw::LoadRenderTexture(mWidth, mHeight);
        rlw::SetTextureFilter(mRenderTexture.texture, rlw::TEXTURE_FILTER_BILINEAR);
        rlw::SetTextureFilter(mRenderTexture.depth, rlw::TEXTURE_FILTER_BILINEAR);
    }

    void Render::ClearRender()
    {
        rlw::BeginTextureMode(mRenderTexture);
        rlw::ClearBackground(Color::WhiteGray);
        rlw::EndTextureMode();
    }

    void Render::BeginRender()
    {
        RenderValues();
        rlw::BeginTextureMode(mRenderTexture);
    }

    void Render::EndRender()
    {
        rlw::EndTextureMode();
    }

    void Render::DrawRender()
    {
        rlw::Texture* texture = &mRenderTexture.texture;

        const float screenWidth = static_cast<float>(rlw::GetScreenWidth());
        const float screenHeight = static_cast<float>(rlw::GetScreenHeight());
        const float textureWidth = static_cast<float>(mRenderTexture.texture.width);
        const float textureHeight = static_cast<float>(mRenderTexture.texture.height);

        mSource = Rectangle{ 0.0f, 0.0f, textureWidth, -textureHeight };
        mDest = Rectangle{ (screenWidth - (mNewWidth * mScale)) * 0.5f, (screenHeight - (mNewHeight * mScale)) * 0.5f, mNewWidth * mScale, mNewHeight * mScale };

#ifdef _DEBUG
        if (rlw::IsKeyReleased(rlw::KEY_K))
        {
            rlw::SetTextureFilter(*texture, rlw::TEXTURE_FILTER_POINT);
            rlw::SetTextureFilter(mRenderTexture.depth, rlw::TEXTURE_FILTER_POINT);

        }
        if (rlw::IsKeyReleased(rlw::KEY_J))
        {
            rlw::SetTextureFilter(*texture, rlw::TEXTURE_FILTER_BILINEAR);
            rlw::SetTextureFilter(mRenderTexture.depth, rlw::TEXTURE_FILTER_BILINEAR);
        }
#endif
        // Draw render texture to screen, properly scaled
        rlw::DrawTexturePro(*texture, mSource, mDest, { 0.0f, 0.0f }, 0.0f, Color::White);
    }

    void Render::Unload()
    {
        if (rlw::IsRenderTextureValid(mRenderTexture))
        {
            rlw::UnloadRenderTexture(mRenderTexture);
            mRenderTexture = rlw::RenderTexture{};
        }
    }

    Vector2f Render::GetRenderScale() const
    {
        const float scaleWidth = static_cast<float>(rlw::GetScreenWidth()) / mWidth;
        const float scaleHeight = static_cast<float>(rlw::GetScreenHeight()) / mHeight;
        return Vector2f{ scaleWidth, scaleHeight };
    }

    Vector2f Render::GetMousePositionRender() const
    {
        return mVirtualMouse;
    }

    int Render::GetWidthRender() const
    {
        return (mIsMaintainAspectRatio) ? static_cast<int>(mWidth) : rlw::GetScreenWidth();
    }

    int Render::GetHeightRender() const
    {
        return (mIsMaintainAspectRatio) ? static_cast<int>(mHeight) : rlw::GetScreenHeight();
    }

}  // namespace ClassicLauncher