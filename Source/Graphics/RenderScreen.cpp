#include "RenderScreen.h"

#include "Graphics/RenderTexture.h"
#include "Helper.h"
#include "Input/Keyboard.h"
#include "Themes/ThemesManager.h"
#include "Utils/Math.h"
#include "Window/RayWindow.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{

    RenderScreen::RenderScreen()
        : mRenderTexture(), mWidth(0), mHeight(0), mNewWidth(0), mNewHeight(0), mScale(1), mIsMaintainAspectRatio(true)
    {
        LOG(LOG_CLASSIC_WARNING, "inicia render screen");
    }

    RenderScreen::~RenderScreen()
    {
    }

    void RenderScreen::UpdateValues()
    {
        //const Vector2f mouse = rlw::GetMousePosition();
        Vector2f mouse = Vector2f{RayWindow::GetMousePosition()};
        
        const auto screenWidth = static_cast<float>(RayWindow::GetScreenWidth());
        const auto screenHeight = static_cast<float>(RayWindow::GetScreenHeight());
        mNewWidth = static_cast<float>(GetWidth());
        mNewHeight = static_cast<float>(GetHeight());

        if (mIsMaintainAspectRatio)
        {
            mScale = Math::Min<float>(screenWidth / mNewWidth, screenHeight / mNewHeight);
            mVirtualMouse.x = (mouse.x - (screenWidth - (mNewWidth * mScale)) * 0.5f) / mScale;
            mVirtualMouse.y = (mouse.y - (screenHeight - (mNewHeight * mScale)) * 0.5f) / mScale;
            mVirtualMouse = Math::VecClamp(mVirtualMouse, Vector2f{0.0f, 0.0f}, Vector2f{mNewWidth, mNewHeight});
        }
        else
        {
            mScale = 1;
            mVirtualMouse.x = (mouse.x / screenWidth) * mWidth;
            mVirtualMouse.y = (mouse.y / screenHeight) * mHeight;
        }

        RayWindow::m_virtualMouse = mVirtualMouse;

        if ( RayWindow::IsResize())
        {
            LOG(LOG_CLASSIC_WARNING, "is resized");
        }
        
        //LOG(LOG_CLASSIC_WARNING, "mouse virtual x: %.2f y: %.2f" , mVirtualMouse.x, mVirtualMouse.y);
    }

    void RenderScreen::Init(const int screenWidth, const int screenHeight)
    {
        const float scale = ThemesManager::GetScaleTexture();
        mWidth = screenWidth * scale;
        mHeight = screenHeight * scale;

        mRenderTexture = std::make_unique<RenderTexture>(static_cast<int>(mWidth), static_cast<int>(mHeight));
        mRenderTexture->SetSmooth(true);
    }

    void RenderScreen::Clear()
    {
        // rlw::BeginTextureMode(mRenderTexture);
        //
        // rlw::EndTextureMode();
    }

    void RenderScreen::BeginRender()
    {
        UpdateValues();
        rlw::BeginTextureMode(*mRenderTexture);
        rlw::ClearBackground(Color::WhiteGray);
    }

    void RenderScreen::EndRender()
    {
        rlw::EndTextureMode();
    }

    void RenderScreen::Draw()
    {

        const auto screenWidth = static_cast<float>(RayWindow::GetScreenWidth());
        const auto screenHeight = static_cast<float>(RayWindow::GetScreenHeight());
        const auto textureWidth = static_cast<float>(mRenderTexture->GetSize().x);
        const auto textureHeight = static_cast<float>(mRenderTexture->GetSize().y);

        mSource = Rectangle{0.0f, 0.0f, textureWidth, -textureHeight};
        mDest = Rectangle{(screenWidth - (mNewWidth * mScale)) * 0.5f,
                          (screenHeight - (mNewHeight * mScale)) * 0.5f,
                          mNewWidth * mScale,
                          mNewHeight * mScale};

#ifdef _DEBUG
        if (Keyboard::IsReleased(Keyboard::Key::K))
        {
            mRenderTexture->SetSmooth(false);
        }
        if (Keyboard::IsReleased(Keyboard::Key::J))
        {
            mRenderTexture->SetSmooth(true);
        }
#endif
        // Draw render texture to screen, properly scaled
        rlw::DrawTexturePro(*mRenderTexture->GetTexture(), mSource, mDest, Vector2f{0.0f, 0.0f}, 0.0f, Color::White);
    }

    void RenderScreen::Unload()
    {
        if (mRenderTexture->IsValid())
        {
            mRenderTexture->Unload();
        }
    }

    Vector2f RenderScreen::GetRenderScale() const
    {
        const float scaleWidth = static_cast<float>(RayWindow::GetScreenWidth()) / mWidth;
        const float scaleHeight = static_cast<float>(RayWindow::GetScreenHeight()) / mHeight;
        return Vector2f{scaleWidth, scaleHeight};
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

} // namespace ClassicLauncher