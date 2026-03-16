#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <string_view>
#include <vector>

#include "ClassicLauncher.h"
#include "Data/Vector2.h"
#include "Utils/ConfigurationManager.h"

namespace ClassicLauncher
{

    struct WindowSpecs
    {
        inline static constexpr int Width = 1280;
        inline static constexpr int Height = 720;
        inline static constexpr std::string_view Title = TITLE PRODUCT_VERSION_STRING;
    };


    class Window
    {
    public:

        friend class WindowSystem;

        Window() = default;
        ~Window();
        void Init(const std::string& title, ConfigurationManager& configManager);
        bool ShouldClose();
        void CloseWindow();
        void CloseScheduledWindow();

        void SetState(unsigned int flags);
        void ClearState(unsigned int flags);
        bool IsState(unsigned int flag);
        void SetSize(int width, int height);
        void SetPosition(int x, int y);
        void SetIcons(const std::vector<std::string>& pathIcons);
        void SetExitKey(int key);
        void SetTargetFPS(int fps);
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
        void SetVirtualMouse(const Vector2f& position);
        void ShowCursor(bool isEnable);
        void MinimizeWindow();
        void RestoreWindow();
        void SetWindowFocused();
        bool IsResize() const;
        void ToggleFullscreen();
        void Update();
        [[nodiscard]] bool IsReady();

        struct Flags
        {
            inline static constexpr unsigned int Vsync = 0x00000040;            // Set to try enabling V-Sync on GPU
            inline static constexpr unsigned int Fullscreen = 0x00000002;       // Set to run program in fullscreen
            inline static constexpr unsigned int Resizable = 0x00000004;        // Set to allow resizable window
            inline static constexpr unsigned int Undecorated = 0x00000008;      // Set to disable window decoration (frame and buttons)
            inline static constexpr unsigned int Hidden = 0x00000080;           // Set to hide window
            inline static constexpr unsigned int Minimized = 0x00000200;        // Set to minimize window (iconify)
            inline static constexpr unsigned int Maximized = 0x00000400;        // Set to maximize window (expanded to monitor)
            inline static constexpr unsigned int Unfocused = 0x00000800;        // Set to window non focused
            inline static constexpr unsigned int Topmost = 0x00001000;          // Set to window always on top
            inline static constexpr unsigned int AlwaysRun = 0x00000100;        // Set to allow windows running while minimized
            inline static constexpr unsigned int Transparent = 0x00000010;      // Set to allow transparent framebuffer
            inline static constexpr unsigned int HighDPI = 0x00002000;          // Set to support HighDPI
            inline static constexpr unsigned int MousePassthrough = 0x00004000; // Set to support mouse passthrough, only supported when FLAG WINDOW UNDECORATED
            inline static constexpr unsigned int BorderlessWindowedMode = 0x00008000; // Set to run program in borderless windowed mode
            inline static constexpr unsigned int Msaa4X = 0x00000020;                 // Set to try enabling MSAA 4X
            inline static constexpr unsigned int Interlaced = 0x00010000;             // Set to try enabling interlaced video format (for V3D)
        };

        struct StatusWindow
        {
            bool close{false};
            bool toggleFullscreen{false};
        };

    private:

        StatusWindow m_status{};
        Vector2f m_virtualMouse{};
        std::string m_title{};
        Vector2i m_size{};
        Vector2i m_position{};
        bool m_isReady{false};
        ConfigurationManager* m_configManager{nullptr};

        bool IsFullScreen();
        void SetConfigFlags(unsigned int flags);
        bool SetFullscreen(bool enable);
        void Unload();
    };

}; // namespace ClassicLauncher

#endif
