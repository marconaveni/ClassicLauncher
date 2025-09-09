#include "Window/RayWindow.h"

#include "raylib.h"

namespace ClassicLauncher
{

    static std::vector<::Image> icons;
    
    RayWindow::~RayWindow()
    {
        Close();
    }

    void RayWindow::Init(int width, int height, const std::string& title)
    {
        m_title = title;
        InitWindow(width, height, title.c_str());
        SetWindowSize(width, height);
        SetWindowState(FLAG_WINDOW_RESIZABLE);
    }

    bool RayWindow::ShouldClose()
    {
        return WindowShouldClose();
    }
    
    void RayWindow::Close()
    {
        CloseWindow();
        for (auto icon: icons) 
        {
            ::UnloadImage(icon);
        }
        icons.clear();
    }

    void RayWindow::SetState(unsigned int flags)
    {
        SetWindowState(flags);
    }

    void RayWindow::ClearState(unsigned int flags)
    {
        ::ClearWindowState(flags);
    }

    bool RayWindow::IsState(unsigned int flag)
    {
        return IsWindowState(flag);
    }

    void RayWindow::SetSize(int width, int height)
    {
        ::SetWindowSize(width, height);
    }

    void RayWindow::SetPosition(int x, int y)
    {
        ::SetWindowPosition(x, y);
    }

    void RayWindow::SetIcons(const std::vector<std::string>& pathIcons)
    {
        icons.reserve(pathIcons.size());
        for (const auto& path : pathIcons) 
        {
            icons.push_back(LoadImage(path.c_str()));
        }
        SetWindowIcons(icons.data(), icons.size());
    }

    void RayWindow::SetExitKey(int key)
    {
        ::SetExitKey(key);
    }

    void RayWindow::SetTargetFPS(int fps)
    {
        ::SetTargetFPS(fps);
    }

    int RayWindow::GetFPS()
    {
        return ::GetFPS();
    }

    float RayWindow::GetFrameTime()
    {
        return ::GetFrameTime();
    }

    int RayWindow::GetScreenWidth()
    {
        return ::GetScreenWidth();
    }

    int RayWindow::GetScreenHeight()
    {
        return ::GetScreenHeight();
    }

    int RayWindow::GetCurrentMonitor()
    {
        return ::GetCurrentMonitor();
    }

    int RayWindow::GetMonitorWidth(int monitor)
    {
        return ::GetMonitorWidth(monitor);
    }

    int RayWindow::GetMonitorHeight(int monitor)
    {
        return ::GetMonitorHeight(monitor);
    }

    Vector2i RayWindow::GetMonitorPosition(int monitor)
    {
        ::Vector2 pos = ::GetMonitorPosition(monitor);
        return {static_cast<int>(pos.x),static_cast<int>(pos.y) };
    }

    bool RayWindow::ToggleFullscreen()
    {
        if (!::IsWindowFullscreen())
        {
            m_position.x = static_cast<int>(GetWindowPosition().x);
            m_position.y = static_cast<int>(GetWindowPosition().y);
            m_size.x = GetScreenWidth();
            m_size.y = GetScreenHeight();
            ::ToggleFullscreen();
            SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            // ::SetConfigFlags(::FLAG_VSYNC_HINT);
            m_isFullScreen = true;
        }
        else
        {
            ::ToggleFullscreen();
            //::SetWindowSize(mSpecification.width, mSpecification.height);
            //::SetWindowPosition(mSpecification.posWindowX, mSpecification.posWindowY);
            //::SetWindowSize(mSpecification.width, mSpecification.height);
            m_isFullScreen = false;
        }
        return m_isFullScreen;
    }

}  // namespace ClassicLauncher