#include "Window/RayWindow.h"
#include "ClassicAssert.h"

namespace ray
{
#include "raylib.h"
}  // namespace ray

namespace ClassicLauncher
{

    static std::vector<ray::Image> icons;

    RayWindow::~RayWindow()
    {
        Close();
    }

    void RayWindow::Init(int width, int height, const std::string& title)
    {
        CLASSIC_ASSERT(!ray::IsWindowReady(), "You not can create another window");

        m_title = title;
        ray::InitWindow(width, height, title.c_str());
        ray::SetWindowSize(width, height);
        ray::SetWindowState(Flags::Resizable);

        m_isReady = ray::IsWindowReady();
    }

    bool RayWindow::ShouldClose()
    {
        return ray::WindowShouldClose();
    }

    void RayWindow::Close()
    {
        ray::CloseWindow();
        Unload();
        m_isReady = ray::IsWindowReady();
    }

    void RayWindow::SetState(unsigned int flags)
    {
        ray::SetWindowState(flags);
    }

    void RayWindow::ClearState(unsigned int flags)
    {
        ray::ClearWindowState(flags);
    }

    bool RayWindow::IsState(unsigned int flag)
    {
        return ray::IsWindowState(flag);
    }

    void RayWindow::SetSize(int width, int height)
    {
        ray::SetWindowSize(width, height);
    }

    void RayWindow::SetPosition(int x, int y)
    {
        ray::SetWindowPosition(x, y);
    }

    void RayWindow::SetIcons(const std::vector<std::string>& pathIcons)
    {
        icons.reserve(pathIcons.size());
        for (const auto& path : pathIcons)
        {
            icons.push_back(ray::LoadImage(path.c_str()));
        }
        SetWindowIcons(icons.data(), icons.size());
    }

    void RayWindow::SetExitKey(int key)
    {
        ray::SetExitKey(key);
    }

    void RayWindow::SetTargetFPS(int fps)
    {
        ray::SetTargetFPS(fps);
    }

    int RayWindow::GetFPS()
    {
        return ray::GetFPS();
    }

    float RayWindow::GetFrameTime()
    {
        return ray::GetFrameTime();
    }

    int RayWindow::GetScreenWidth()
    {
        return ray::GetScreenWidth();
    }

    int RayWindow::GetScreenHeight()
    {
        return ray::GetScreenHeight();
    }

    int RayWindow::GetCurrentMonitor()
    {
        return ray::GetCurrentMonitor();
    }

    int RayWindow::GetMonitorWidth(int monitor)
    {
        return ray::GetMonitorWidth(monitor);
    }

    int RayWindow::GetMonitorHeight(int monitor)
    {
        return ray::GetMonitorHeight(monitor);
    }

    Vector2i RayWindow::GetMonitorPosition(int monitor)
    {
        ray::Vector2 pos = ray::GetMonitorPosition(monitor);
        return { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    }

    bool RayWindow::ToggleFullscreen()
    {
#ifdef _WIN32
        const bool isNotFullscreen = !ray::IsWindowState(Flags::Undecorated);
#else
        const bool isNotFullscreen = !ray::IsWindowFullscreen();
#endif

        if (isNotFullscreen)
        {
            m_position.x = static_cast<int>(ray::GetWindowPosition().x);
            m_position.y = static_cast<int>(ray::GetWindowPosition().y);
            m_size.x = GetScreenWidth();
            m_size.y = GetScreenHeight();
        }
#ifdef _WIN32
        if (isNotFullscreen)
        {
            SetState(Flags::Undecorated);
            SetSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            const Vector2f positionMonitor(GetMonitorPosition(GetCurrentMonitor()));
            SetPosition((int)positionMonitor.x, (int)positionMonitor.y);
            m_isFullScreen = true;
        }
        else
        {
            SetSize(m_size.x, m_size.y);
            SetPosition(m_position.x, m_position.y);
            ClearState(Flags::Undecorated);
            m_isFullScreen = false;
        }
#else
        if (isNotFullscreen)
        {
            ray::ToggleFullscreen();
            SetSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            m_isFullScreen = true;
        }
        else
        {
            ray::ToggleFullscreen();
            //::SetWindowSize(mSpecification.width, mSpecification.height);
            //::SetWindowPosition(mSpecification.posWindowX, mSpecification.posWindowY);
            //::SetWindowSize(mSpecification.width, mSpecification.height);
            m_isFullScreen = false;
        }
#endif
        return m_isFullScreen;
    }

    void RayWindow::SetConfigFlags(unsigned int flags)
    {
        ray::SetConfigFlags(flags);
    }

    void RayWindow::Unload()
    {
        for (auto icon : icons)
        {
            ray::UnloadImage(icon);
        }
        icons.clear();
    }

}  // namespace ClassicLauncher
