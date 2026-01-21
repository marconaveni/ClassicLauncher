#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <string_view>
#include <vector>
#include "Data/Vector2.h"
#include "ClassicLauncher.h"

namespace ClassicLauncher
{

    struct WindowSpecs
    {
        inline static constexpr int Width = 1280;
        inline static constexpr int Height = 720;
        inline static constexpr std::string_view Title = TITLE  PRODUCT_VERSION_STRING;  
    };
    

    class ConfigurationManager;

    class Window
    {
    public:

        virtual ~Window() = default;

        virtual void Init(int width, int height, const std::string& title, ConfigurationManager& configManager) = 0;
        virtual bool ShouldClose() = 0;
        virtual void Close() = 0;

        virtual void SetState(unsigned int flags) = 0;
        virtual void ClearState(unsigned int flags) = 0;
        virtual bool IsState(unsigned int flag) = 0;
        virtual void SetSize(int width, int height) = 0;
        virtual void SetPosition(int x, int y) = 0;
        virtual void SetIcons(const std::vector<std::string>& pathIcons) = 0;
        virtual void SetExitKey(int key) = 0;
        virtual void SetTargetFPS(int fps) = 0;
        virtual int GetFPS() const = 0;
        virtual float GetFrameTime() const = 0;
        virtual int GetScreenWidth() const = 0;
        virtual int GetScreenHeight() const = 0;
        virtual int GetCurrentMonitor() const = 0;
        virtual int GetMonitorWidth(int monitor) const = 0;
        virtual int GetMonitorHeight(int monitor) const = 0;
        virtual Vector2i GetMonitorPosition(int monitor) const = 0;
        virtual Vector2i GetMousePosition() const = 0;
        virtual Vector2f GetVirtualMouse() const = 0;
        virtual void SetVirtualMouse(const Vector2f& position) = 0;
        virtual void ShowCursor(bool isEnable) = 0;
        virtual void MinimizeWindow() = 0;
        virtual void RestoreWindow() = 0;
        virtual void SetWindowFocused() = 0;
        virtual bool IsResize() const = 0;
        virtual void PoolEvents() = 0;
    };

};  // namespace ClassicLauncher

#endif
