#include "Window/RayWindow.h"

#include "ClassicAssert.h"
#include "Helper.h"
#include "Input/InputManager.h"
#include "RayWindow.h"

namespace ray
{
#include "raylib.h"
} // namespace ray

namespace ClassicLauncher
{

    static std::vector<ray::Image> icons;
    RayWindow::~RayWindow()
    {
        Close();
    }

    void RayWindow::Init(int width, int height, const std::string& title, ConfigurationManager& configManager)
    {
        //CLASSIC_ASSERT(!ray::IsWindowReady(), "You not can create another window");

        m_configManager = &configManager;

        if (m_configManager->GetVSync())
        {
            SetConfigFlags(Flags::Vsync); // vsync only enable in fullscreen set before InitWindow
        }

        m_title = title;
        ray::InitWindow(m_configManager->GetWidthWindow(), m_configManager->GetHeightWindow(), title.c_str());
       // ray::SetWindowSize(width, height);
        ray::SetWindowState(Flags::Resizable | Flags::AlwaysRun);
        SetTargetFPS(m_configManager->GetTargetFps());
        SetFullscreen(m_configManager->GetFullscreen());

#ifndef _DEBUG
        //SetExitKey(0);
#endif

        m_isReady = ray::IsWindowReady();
    }

    bool RayWindow::ShouldClose()
    {
        return ray::WindowShouldClose();
    }

    void RayWindow::Close()
    {
        ClearState(Flags::Undecorated | Flags::Resizable | Flags::AlwaysRun);
        ray::CloseWindow();
        Unload();
        m_isReady = ray::IsWindowReady();
        m_configManager = nullptr;
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
        ray::SetWindowIcons(icons.data(), icons.size());
    }

    void RayWindow::SetExitKey(int key)
    {
        ray::SetExitKey(key);
    }

    void RayWindow::SetTargetFPS(int fps)
    {
        ray::SetTargetFPS(fps);
    }

    int RayWindow::GetFPS() const
    {
        return ray::GetFPS();
    }

    float RayWindow::GetFrameTime() const
    {
        return ray::GetFrameTime();
    }

    int RayWindow::GetScreenWidth() const
    {
        return ray::GetScreenWidth();
    }

    int RayWindow::GetScreenHeight() const
    {
        return ray::GetScreenHeight();
    }

    int RayWindow::GetCurrentMonitor() const
    {
        return ray::GetCurrentMonitor();
    }

    int RayWindow::GetMonitorWidth(int monitor) const
    {
        return ray::GetMonitorWidth(monitor);
    }

    int RayWindow::GetMonitorHeight(int monitor) const
    {
        return ray::GetMonitorHeight(monitor);
    }

    Vector2i RayWindow::GetMonitorPosition(int monitor) const
    {
        ray::Vector2 pos = ray::GetMonitorPosition(monitor);
        return Vector2i{static_cast<int>(pos.x), static_cast<int>(pos.y)};
    }

    Vector2i RayWindow::GetMousePosition() const
    {
        ray::Vector2 pos = ray::GetMousePosition();
        return Vector2i{static_cast<int>(pos.x), static_cast<int>(pos.y)};
    }

    Vector2f RayWindow::GetVirtualMouse() const
    {
        return m_virtualMouse;
    }

    void RayWindow::SetVirtualMouse(const Vector2f& position)
    {
        m_virtualMouse = position;
    }

    void RayWindow::ShowCursor(bool isEnable)
    {
        if (isEnable)
        {
            ray::ShowCursor();
            return;
        }
        ray::HideCursor();            
    }

    void RayWindow::MinimizeWindow()
    {
        ray::MinimizeWindow();
    }

    void RayWindow::RestoreWindow()
    {
        ray::RestoreWindow();
    }

    void RayWindow::SetWindowFocused()
    {
        ray::SetWindowFocused();
    }

    bool RayWindow::ToggleFullscreen()
    {
        return SetFullscreen(!IsFullScreen());
    }

    bool RayWindow::SetFullscreen(bool enableFullscreen)
    {       
        bool isFullScreen = IsFullScreen();

        if (enableFullscreen && !isFullScreen)
        {
#ifdef _WIN32
            m_position.x = static_cast<int>(ray::GetWindowPosition().x);
            m_position.y = static_cast<int>(ray::GetWindowPosition().y);
            m_size.x = GetScreenWidth();
            m_size.y = GetScreenHeight();
            SetState(Flags::Undecorated);
            SetSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            const Vector2i positionMonitor(GetMonitorPosition(GetCurrentMonitor()));
            SetPosition(positionMonitor.x, positionMonitor.y);
#else
            ray::ToggleFullscreen();
            SetSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
#endif
        }
        else if (!enableFullscreen && isFullScreen)
        {
#ifdef _WIN32
            SetSize(m_size.x, m_size.y);
            SetPosition(m_position.x, m_position.y);
            ClearState(Flags::Undecorated);
#else
            ray::ToggleFullscreen();
#endif
        }
        ShowCursor(!IsFullScreen());

        m_configManager->SetFullscreen(IsFullScreen());
        m_configManager->SaveConfiguration();
        LOG(LOG_CLASSIC_DEBUG, TEXT("Saved is fullscreen config.ini with value %s", TEXTBOOL(m_configManager->GetFullscreen())));

        return IsFullScreen();
    }

    bool RayWindow::IsFullScreen()
    {
#ifdef _WIN32
        return ray::IsWindowState(Flags::Undecorated);
#else
        return ray::IsWindowFullscreen();
#endif
    }

    void RayWindow::SetConfigFlags(unsigned int flags)
    {
        ray::SetConfigFlags(flags);
    }

    bool RayWindow::IsResize() const
    {
        return ray::IsWindowResized();
    }

    bool RayWindow::IsReady()
    {
        return ray::IsWindowReady();
    }

    void RayWindow::Unload()
    {
        for (auto icon : icons)
        {
            ray::UnloadImage(icon);
        }
        icons.clear();
    }

    void RayWindow::PoolEvents()
    {
        if (Keyboard::IsReleased(Keyboard::F11) ||
            (Keyboard::IsDown(Keyboard::LEFT_ALT) && Keyboard::IsReleased(Keyboard::ENTER)))
        {
            ToggleFullscreen();
        }
    }

} // namespace ClassicLauncher
