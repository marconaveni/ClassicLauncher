#include "Window/Window.h"

#include "ClassicAssert.h"
#include "Helper.h"
#include "Input/InputManager.h"

#include "Ray.h"


namespace ClassicLauncher
{

    static std::vector<ray::Image> icons;
    Window::~Window()
    {
        CloseWindow();
    }

    void Window::Init(const std::string& title, ConfigurationManager& configManager)
    {
        m_configManager = &configManager;

        if (m_configManager->GetVSync())
        {
            SetConfigFlags(Flags::Vsync); // vsync only enable in fullscreen set before InitWindow
        }

        m_title = title;
        ray::InitWindow(m_configManager->GetWidthWindow(), m_configManager->GetHeightWindow(), title.c_str());
        SetState(Flags::Resizable | Flags::AlwaysRun);
        SetTargetFPS(m_configManager->GetTargetFps());
        SetFullscreen(m_configManager->GetFullscreen());
        SetExitKey(0);

        m_isReady = ray::IsWindowReady();
    }

    bool Window::ShouldClose()
    {
        return ray::WindowShouldClose() || m_status.close;
    }

    void Window::CloseWindow()
    {
        LOG(LogDebug, "Closing Window");
        if (ray::IsWindowReady())
        {
            ClearState(Flags::Undecorated | Flags::Resizable | Flags::AlwaysRun);
            ray::CloseWindow();
            Unload();
            m_isReady = ray::IsWindowReady();
            LOG(LogDebug, "Window is Closed %s", TEXTBOOL(!m_isReady));
            m_configManager = nullptr;
        }
    }

    void Window::CloseScheduledWindow()
    {
        m_status.close = true;
    }

    void Window::SetState(unsigned int flags)
    {
        ray::SetWindowState(flags);
    }

    void Window::ClearState(unsigned int flags)
    {
        ray::ClearWindowState(flags);
    }

    bool Window::IsState(unsigned int flag)
    {
        return ray::IsWindowState(flag);
    }

    void Window::SetSize(int width, int height)
    {
        ray::SetWindowSize(width, height);
    }

    void Window::SetPosition(int x, int y)
    {
        ray::SetWindowPosition(x, y);
    }

    void Window::SetIcons(const std::vector<std::string>& pathIcons)
    {
        icons.reserve(pathIcons.size());
        for (const auto& path : pathIcons)
        {
            icons.push_back(ray::LoadImage(path.c_str()));
        }
        ray::SetWindowIcons(icons.data(), icons.size());
    }

    void Window::SetExitKey(int key)
    {
        ray::SetExitKey(key);
    }

    void Window::SetTargetFPS(int fps)
    {
        ray::SetTargetFPS(fps);
    }

    int Window::GetFPS() const
    {
        return ray::GetFPS();
    }

    float Window::GetFrameTime() const
    {
        return ray::GetFrameTime();
    }

    int Window::GetScreenWidth() const
    {
        return ray::GetScreenWidth();
    }

    int Window::GetScreenHeight() const
    {
        return ray::GetScreenHeight();
    }

    int Window::GetCurrentMonitor() const
    {
        return ray::GetCurrentMonitor();
    }

    int Window::GetMonitorWidth(int monitor) const
    {
        return ray::GetMonitorWidth(monitor);
    }

    int Window::GetMonitorHeight(int monitor) const
    {
        return ray::GetMonitorHeight(monitor);
    }

    Vector2i Window::GetMonitorPosition(int monitor) const
    {
        ray::Vector2 pos = ray::GetMonitorPosition(monitor);
        return Vector2i{static_cast<int>(pos.x), static_cast<int>(pos.y)};
    }

    Vector2i Window::GetMousePosition() const
    {
        ray::Vector2 pos = ray::GetMousePosition();
        return Vector2i{static_cast<int>(pos.x), static_cast<int>(pos.y)};
    }

    Vector2f Window::GetVirtualMouse() const
    {
        return m_virtualMouse;
    }

    void Window::SetVirtualMouse(const Vector2f& position)
    {
        m_virtualMouse = position;
    }

    void Window::ShowCursor(bool isEnable)
    {
        if (isEnable)
        {
            ray::ShowCursor();
            return;
        }
        ray::HideCursor();
    }

    void Window::MinimizeWindow()
    {
        ray::MinimizeWindow();
    }

    void Window::RestoreWindow()
    {
        ray::RestoreWindow();
    }

    void Window::SetWindowFocused()
    {
        ray::SetWindowFocused();
    }

    void Window::ToggleFullscreen()
    {
        m_status.toggleFullscreen = true;
    }

    bool Window::SetFullscreen(bool enableFullscreen)
    {
#ifdef PLATFORM_RPI
        ShowCursor(false);
        return true;
#else

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
    #endif //_WIN32
        }
        ShowCursor(!IsFullScreen());

        m_configManager->SetFullscreen(IsFullScreen());
        m_configManager->SaveConfiguration();
        LOG(LogDebug, TEXT("Saved is fullscreen config.ini with value %s", TEXTBOOL(m_configManager->GetFullscreen())));

        return IsFullScreen();
#endif     // PLATFORM_RPI
    }

    bool Window::IsFullScreen()
    {
#ifdef _WIN32
        return ray::IsWindowState(Flags::Undecorated);
#else
        return ray::IsWindowFullscreen();
#endif
    }

    void Window::SetConfigFlags(unsigned int flags)
    {
        ray::SetConfigFlags(flags);
    }

    bool Window::IsResize() const
    {
        return ray::IsWindowResized();
    }

    bool Window::IsReady()
    {
        return ray::IsWindowReady();
    }

    void Window::Unload()
    {
        for (auto icon : icons)
        {
            ray::UnloadImage(icon);
        }
        icons.clear();
    }

    void Window::Update()
    {
        if (m_status.toggleFullscreen)
        {
            m_status.toggleFullscreen = false;
            SetFullscreen(!IsFullScreen());
        }
    }

} // namespace ClassicLauncher
