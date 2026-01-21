#include "Window/WindowSystem.h"

namespace ClassicLauncher
{

    WindowSystem& WindowSystem::Get()
    {
        static WindowSystem instance;
        return instance;
    }

    void WindowSystem::Bind(Window* window)
    {
        m_window = window;
    }

    Window& WindowSystem::GetWindow() const
    {
        CLASSIC_ASSERT(m_window != nullptr, "WindowSystem is not bound to a window");
        return *m_window;
    }

    int WindowSystem::GetFPS() const
    {
        return GetWindow().GetFPS();
    }

    float WindowSystem::GetFrameTime() const
    {
        return GetWindow().GetFrameTime();
    }

    int WindowSystem::GetScreenWidth() const
    {
        return GetWindow().GetScreenWidth();
    }

    int WindowSystem::GetScreenHeight() const
    {
        return GetWindow().GetScreenHeight();
    }

    int WindowSystem::GetCurrentMonitor() const
    {
        return GetWindow().GetCurrentMonitor();
    }

    int WindowSystem::GetMonitorWidth(int monitor) const
    {
        return GetWindow().GetMonitorWidth(monitor);
    }

    int WindowSystem::GetMonitorHeight(int monitor) const
    {
        return GetWindow().GetMonitorHeight(monitor);
    }

    Vector2i WindowSystem::GetMonitorPosition(int monitor) const
    {
        return GetWindow().GetMonitorPosition(monitor);
    }

    Vector2i WindowSystem::GetMousePosition() const
    {
        return GetWindow().GetMousePosition();
    }

    Vector2f WindowSystem::GetVirtualMouse() const
    {
        return GetWindow().GetVirtualMouse();
    }

    void WindowSystem::SetVirtualMouse(const Vector2f& position) const
    {
        GetWindow().SetVirtualMouse(position);
    }

    void WindowSystem::ShowCursor(bool isEnable) const
    {
        GetWindow().ShowCursor(isEnable);
    }

    void WindowSystem::MinimizeWindow() const
    {
        GetWindow().MinimizeWindow();
    }

    void WindowSystem::RestoreWindow() const
    {
        GetWindow().RestoreWindow();
    }

    void WindowSystem::SetWindowFocused() const
    {
        GetWindow().SetWindowFocused();
    }

    bool WindowSystem::IsResize() const
    {
        return GetWindow().IsResize();
    }

} // namespace ClassicLauncher
