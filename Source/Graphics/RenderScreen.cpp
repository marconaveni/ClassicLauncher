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

    void RenderScreen::UpdateValues()
    {
        //const Vector2f mouse = rlw::GetMousePosition();
        Vector2f mouse = Vector2f{RayWindow::GetMousePosition()};
        
        const auto screenWidth = static_cast<float>(RayWindow::GetScreenWidth());
        const auto screenHeight = static_cast<float>(RayWindow::GetScreenHeight());
        m_newWidth = static_cast<float>(GetWidth());
        m_newHeight = static_cast<float>(GetHeight());

        if (m_isMaintainAspectRatio)
        {
            m_scale = Math::Min<float>(screenWidth / m_newWidth, screenHeight / m_newHeight);
            m_virtualMouse.x = (mouse.x - (screenWidth - (m_newWidth * m_scale)) * 0.5f) / m_scale;
            m_virtualMouse.y = (mouse.y - (screenHeight - (m_newHeight * m_scale)) * 0.5f) / m_scale;
            m_virtualMouse = Math::VecClamp(m_virtualMouse, Vector2f{0.0f, 0.0f}, Vector2f{m_newWidth, m_newHeight});
        }
        else
        {
            m_scale = 1;
            m_virtualMouse.x = (mouse.x / screenWidth) * m_width;
            m_virtualMouse.y = (mouse.y / screenHeight) * m_height;
        }

        RayWindow::m_virtualMouse = m_virtualMouse;

        if ( RayWindow::IsResize())
        {
            LOG(LOG_CLASSIC_WARNING, "is resized");
        }
        
        //LOG(LOG_CLASSIC_WARNING, "mouse virtual x: %.2f y: %.2f" , mVirtualMouse.x, mVirtualMouse.y);
    }

    void RenderScreen::Init(const int screenWidth, const int screenHeight)
    {
        const float renderScale = ThemesManager::GetScaleRenderer();
        m_width = screenWidth * renderScale;
        m_height = screenHeight * renderScale;

        m_renderTexture = std::make_unique<RenderTexture>(static_cast<int>(m_width), static_cast<int>(m_height));
        m_renderTexture->SetSmooth(true);
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
        rlw::BeginTextureMode(*m_renderTexture);
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
        const auto textureWidth = static_cast<float>(m_renderTexture->GetSize().width);
        const auto textureHeight = static_cast<float>(m_renderTexture->GetSize().height);

        m_source = Rectangle{0.0f, 0.0f, textureWidth, -textureHeight};
        m_dest = Rectangle{(screenWidth - (m_newWidth * m_scale)) * 0.5f,
                          (screenHeight - (m_newHeight * m_scale)) * 0.5f,
                          m_newWidth * m_scale,
                          m_newHeight * m_scale};

#ifdef _DEBUG
        if (Keyboard::IsReleased(Keyboard::Key::K))
        {
            m_renderTexture->SetSmooth(false);
        }
        if (Keyboard::IsReleased(Keyboard::Key::J))
        {
            m_renderTexture->SetSmooth(true);
        }
#endif
        // Draw render texture to screen, properly scaled
        rlw::DrawTexturePro(*m_renderTexture->GetTexture(), m_source, m_dest, Vector2f{0.0f, 0.0f}, 0.0f, Color::White);
    }

    void RenderScreen::Unload()
    {
        if (m_renderTexture->IsValid())
        {
            m_renderTexture->Unload();
        }
    }

    Vector2f RenderScreen::GetRenderScale() const
    {
        const float scaleWidth = static_cast<float>(RayWindow::GetScreenWidth()) / m_width;
        const float scaleHeight = static_cast<float>(RayWindow::GetScreenHeight()) / m_height;
        return Vector2f{scaleWidth, scaleHeight};
    }

    Vector2f RenderScreen::GetMousePositionRender() const
    {
        return m_virtualMouse;
    }

    int RenderScreen::GetWidth() const
    {
        return (m_isMaintainAspectRatio) ? static_cast<int>(m_width) : RayWindow::GetScreenWidth();
    }

    int RenderScreen::GetHeight() const
    {
        return (m_isMaintainAspectRatio) ? static_cast<int>(m_height) : RayWindow::GetScreenHeight();
    }

} // namespace ClassicLauncher