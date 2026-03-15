#ifndef WINDOW_SYSTEM_H
#define WINDOW_SYSTEM_H

#include "ClassicAssert.h"
#include "Window/Window.h"

namespace ClassicLauncher
{

    class WindowSystem
    {
    public:

        static WindowSystem& Get();
        void Bind(Window* window);
        Window& GetWindow() const;
        int GetFPS() const;
        float GetFrameTime() const;
        int GetScreenWidth() const;
        int GetScreenHeight() const;
        int GetCurrentMonitor() const;
        int GetMonitorWidth(int monitor) const;
        int GetMonitorHeight(int monitor) const;
        Vector2i GetMonitorPosition(int monitor) const;
        Vector2i GetMousePosition() const;
        Vector2f GetVirtualMouse() const;
        void SetVirtualMouse(const Vector2f& position) const;
        void ShowCursor(bool isEnable) const;
        void MinimizeWindow() const;
        void RestoreWindow() const;
        void SetWindowFocused() const;
        bool IsResize() const;
        void ToggleFullscreen();
        void Close();

    private:

        Window* m_window = nullptr;
    };

} // namespace ClassicLauncher

#endif
