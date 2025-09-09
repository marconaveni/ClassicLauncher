#include "RenderScreen.h"

#include "Utils/Math.h"
#include "Themes/Themes.h"
#include "rl_wrap.h"
#include "Window/RayWindow.h"



namespace ClassicLauncher
{

    RenderScreen::RenderScreen()
        : mRenderTexture(), mWidth(0), mHeight(0), mNewWidth(0), mNewHeight(0), mScale(1), mIsMaintainAspectRatio(true)
    {
    }

    void RenderScreen::RenderValues()
    {
        const Vector2f mouse = rlw::GetMousePosition();
        const auto screenWidth = static_cast<float>(RayWindow::GetScreenWidth());
        const auto screenHeight = static_cast<float>(RayWindow::GetScreenHeight());
        mNewWidth = static_cast<float>(GetWidth());
        mNewHeight = static_cast<float>(GetHeight());

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

    void RenderScreen::Init(const int screenWidth, const int screenHeight)
    {
        const float scale = Themes::GetScaleTexture();
        mWidth = screenWidth * scale;
        mHeight = screenHeight * scale;

        mRenderTexture = rlw::LoadRenderTexture(mWidth, mHeight);
        rlw::SetTextureFilter(mRenderTexture.texture, rlw::TEXTURE_FILTER_BILINEAR);
        rlw::SetTextureFilter(mRenderTexture.depth, rlw::TEXTURE_FILTER_BILINEAR);
    }

    void RenderScreen::Clear()
    {
        //rlw::BeginTextureMode(mRenderTexture);
        //
        //rlw::EndTextureMode();
    }

    void RenderScreen::BeginRender()
    {
        RenderValues();
        rlw::BeginTextureMode(mRenderTexture);
        rlw::ClearBackground(Color::WhiteGray);
    }

    void RenderScreen::EndRender()
    {
        rlw::EndTextureMode();
    }

    void RenderScreen::Draw()
    {
        rlw::Texture2D* texture = &mRenderTexture.texture;

        const auto screenWidth = static_cast<float>(RayWindow::GetScreenWidth());
        const auto screenHeight = static_cast<float>(RayWindow::GetScreenHeight());
        const auto textureWidth = static_cast<float>(mRenderTexture.texture.width);
        const auto textureHeight = static_cast<float>(mRenderTexture.texture.height);

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

    void RenderScreen::Unload()
    {
        if (rlw::IsRenderTextureValid(mRenderTexture))
        {
            rlw::UnloadRenderTexture(mRenderTexture);
            mRenderTexture = rlw::RenderTexture2D{};
        }
    }

    Vector2f RenderScreen::GetRenderScale() const
    {
        const float scaleWidth = static_cast<float>(RayWindow::GetScreenWidth()) / mWidth;
        const float scaleHeight = static_cast<float>(RayWindow::GetScreenHeight()) / mHeight;
        return Vector2f{ scaleWidth, scaleHeight };
    }

    Vector2f RenderScreen::GetMousePositionRender() const
    {
        return mVirtualMouse;
    }

    int RenderScreen::GetWidth() const
    {
        return (mIsMaintainAspectRatio) ? static_cast<int>(mWidth) : RayWindow::GetScreenWidth();
    }

    int RenderScreen::GetHeight() const
    {
        return (mIsMaintainAspectRatio) ? static_cast<int>(mHeight) : RayWindow::GetScreenHeight();
    }

}  // namespace ClassicLauncher